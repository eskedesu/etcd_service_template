#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <userver/etcd/component.hpp>

namespace etcd_service_template {

void AppendSetKeyValue(userver::components::ComponentList& component_list);

}  // namespace etcd_service_template
