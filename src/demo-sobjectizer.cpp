#include <condition_variable>
#include <iostream>
#include <mutex>
#include <so_5/all.hpp>
using namespace std::literals;

// tmp
std::mutex m;
std::condition_variable cond;
bool global_done = false;

class main_actor final : public so_5::agent_t {
  using so_5::agent_t::agent_t;
  const so_5::mbox_t mbox_;
  so_5::timer_id_t timer_;

  struct ping final : public so_5::signal_t {};

  void on_timer(mhood_t<ping>) const { so_5::send<int>(mbox_, 42); }

public:
  main_actor(context_t ctx, so_5::mbox_t mbox)
      : so_5::agent_t{std::move(ctx)}, mbox_{std::move(mbox)} {}

  void so_define_agent() override {
    so_subscribe_self().event(&main_actor::on_timer);
  }

  void so_evt_start() override {
    std::cout << "main_actor started" << std::endl;
    so_5::send_delayed<ping>(*this, 999ms);
  }
};

class peer_actor final : public so_5::agent_t {
  const so_5::mbox_t mbox_;
  const std::string name_;

  void on_value(mhood_t<int> what) const {
    std::cout << "peer_actor received: " << *what << std::endl;
    global_done = true;
    cond.notify_all();
  }

public:
  peer_actor(context_t ctx, so_5::mbox_t mbox)
      : so_5::agent_t{std::move(ctx)}, mbox_{std::move(mbox)} {}

  void so_define_agent() override {
    so_subscribe(mbox_).event(&peer_actor::on_value);
  }
};

void demo_sobjectizer() {
  so_5::launch([](so_5::environment_t &env) {
    auto mbox = env.create_mbox();
    env.introduce_coop([mbox](so_5::coop_t &coop) {
      coop.make_agent<main_actor>(mbox);
      coop.make_agent<peer_actor>(mbox);
    });

    // wait for the scenario to finish
    std::unique_lock lk{m};
    cond.wait(lk, [] { return global_done; });

    env.stop();
  });
}
