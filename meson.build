# PYBIND11: /Users/xfeloper/.conda/envs/esme/bin/pybind11-config
# PKG_CONFIG: export PKG_CONFIG_PATH=/local/Darwin-x86_64/lib/pkgconfig

project('llpydoocs',
    [ 'cpp' ],
    version : '0.1',
    default_options : [
        'cpp_std=c++14',
    ],
)

# Source files
src = ['src/llpydoocs.cpp']

# Python
python_inst = import('python').find_installation('python3')

pybind11_config = find_program('pybind11-config')
pybind11_config_ret = run_command(pybind11_config, ['--includes'])
pybind11 = declare_dependency(
    include_directories: [pybind11_config_ret.stdout().split('-I')[-1].strip()],
)

# Dependencies
dep = [
    dependency('doocs-doocsapi'),
    dependency('pybind11'),
]

python_inst.extension_module(meson.project_name(),
    sources : src,
    dependencies : dep,
    link_language : 'cpp',
    override_options: [
        'cpp_rtti=true',
    ]
)
