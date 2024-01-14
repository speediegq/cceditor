project('cceditor','cpp',version : '"0.1.0"',default_options : ['warning_level=3'])

project_source_files = [
  'src/cceditor.cpp',
  'src/CCData.cpp',
]

project_dependencies = [
  dependency('zlib'),
  dependency('wxwidgets'),
  dependency('boost', modules : ['regex']),
]

build_args = [
  '-DVERSION=' + meson.project_version(),
]

project_target = executable(
  meson.project_name(),
  project_source_files, install : true,
  dependencies: project_dependencies,
  c_args : build_args,
)

test(meson.project_name(), project_target)
