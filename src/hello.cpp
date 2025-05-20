#include "hello.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/chaotic/io/userver/crypto/base64/string64.hpp>

namespace etcd_service_template {

namespace {

class Hello final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-hello";

  Hello(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        etcd_client_ptr_(
            component_context
                .FindComponent<userver::etcd::Component>("etcd-client")
                .GetClient()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    etcd_client_ptr_->Put("key", "value1");
    etcd_client_ptr_->Put("key2", "value2");
    etcd_client_ptr_->Put("key3", "value3");
    etcd_client_ptr_->Put("lol", "value4");
    etcd_client_ptr_->Put("lol2", "value4");
    etcd_client_ptr_->Put("lol3", "value4");
    etcd_client_ptr_->Put("loqqq", "value4");
    etcd_client_ptr_->Put("loaaa", "value4");

    const auto range_result = etcd_client_ptr_->Range("key");
    LOG_DEBUG() << "Range result " << range_result.size();

    const auto range_result_2 = etcd_client_ptr_->Range("");
    LOG_DEBUG() << "Range result " << range_result_2.size();

    const auto range_result_3 = etcd_client_ptr_->Range("lol");
    for (const auto& i : range_result_3) {
      LOG_DEBUG() << "Range result: " << i.key << " - " << i.value;
    }
    LOG_DEBUG() << "Range result " << range_result_3.size();

    const auto range_result_4 = etcd_client_ptr_->Range("lo");
    LOG_DEBUG() << "Range result " << range_result_4.size();


    const auto get_result = etcd_client_ptr_->Get("key");
    LOG_DEBUG() << "Get result " << get_result.value_or("No value");

    etcd_client_ptr_->Delete("key");
    etcd_client_ptr_->Delete("key3");

    const auto range_result_after_deletion = etcd_client_ptr_->Range("key");
    LOG_DEBUG() << "Range result after deletion "
                << range_result_after_deletion.size();

    auto kek = etcd_client_ptr_->StartWatch("key");

    int i = 3;
    while (i != 0) {
      auto event = kek.GetEvent();
      LOG_DEBUG() << "event info " << event.key << " " << event.value << " "
                  << event.version;
      --i;
    }
    return etcd_service_template::SayHelloTo(request.GetArg("name"));
  }

 private:
  userver::etcd::ClientPtr etcd_client_ptr_;
};

}  // namespace

std::string SayHelloTo(std::string_view name) {
  if (name.empty()) {
    name = "unknown user";
  }

  return fmt::format("Hello, {}!\n", name);
}

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
}

}  // namespace etcd_service_template
