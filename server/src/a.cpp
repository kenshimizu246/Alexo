#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

enum action_type {
  SUBSCRIBE,
  UNSUBSCRIBE,
  MESSAGE
};

struct action {
  action(action_type t, connection_hdl h) : type(t), hdl(h) {}

  action_type type;
  websocketpp::connection_hdl hdl;
  server::message_ptr msg;
};

int main(){
  std::cout << "test..." << std::endl;
}
