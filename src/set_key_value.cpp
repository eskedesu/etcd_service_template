#include "set_key_value.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>

namespace etcd_service_template {

namespace {

class SetKeyValue final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-set-key-value";

  SetKeyValue(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        etcd_client_ptr_(
            component_context
                .FindComponent<userver::etcd::Component>("etcd-client")
                .GetClient()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    etcd_client_ptr_->Put("key", request.GetArg("value"));

    const auto range_result = etcd_client_ptr_->Range("key");
    LOG_DEBUG() << "Range result after set " << range_result.size();

    return std::string();
  }

 private:
  userver::etcd::ClientPtr etcd_client_ptr_;
};

}  // namespace

void AppendSetKeyValue(userver::components::ComponentList& component_list) {
  component_list.Append<SetKeyValue>();
}

}  // namespace etcd_service_template
