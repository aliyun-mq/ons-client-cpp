workspace(name = "com_aliyun_ons")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "org_apache_rocketmq",
    sha256 = "5ed7b30186c9088c484ecd16b935988ac9daf0e6280fd4319373ba447ef9f694",
    urls = [
        "https://github.com/lizhanhui/rocketmq-client-cpp/archive/refs/tags/v1.0-alpha5.tar.gz",
    ],
    strip_prefix = "rocketmq-client-cpp-1.0-alpha5",
)

load("@org_apache_rocketmq//bazel:rocketmq_deps.bzl", "rocketmq_deps")
rocketmq_deps()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps", "grpc_test_only_deps")
grpc_deps()
grpc_test_only_deps()
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()


load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_toolchains", "rules_proto_grpc_repos")
rules_proto_grpc_toolchains()
rules_proto_grpc_repos()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()