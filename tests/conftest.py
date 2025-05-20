import typing


import pytest


pytest_plugins = ['pytest_userver.plugins.core']


@pytest.fixture(scope='session')
def allowed_url_prefixes_extra() -> typing.List[str]:
    return ["http://localhost:2379"]


@pytest.fixture(scope='session')
def userver_config_http_client(
    mockserver_info,
    mockserver_ssl_info,
    allowed_url_prefixes_extra,
):
    def patch_config(config, config_vars):
        components: dict = config['components_manager']['components']
        if not {'http-client', 'testsuite-support'}.issubset(
            components.keys(),
        ):
            return
        http_client = components['http-client'] or {}
        http_client['testsuite-enabled'] = False

        allowed_urls = [mockserver_info.base_url]
        if mockserver_ssl_info:
            allowed_urls.append(mockserver_ssl_info.base_url)
        allowed_urls += allowed_url_prefixes_extra
        http_client['testsuite-allowed-url-prefixes'] = allowed_urls

    return patch_config
