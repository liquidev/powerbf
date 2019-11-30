libpbf_sources = [
  'pbf_args.c',
  'pbf_chunk.c',
  'pbf_compiler.c',
  'pbf_vm.c'
]

libpbf = library('powerbf', libpbf_sources)

libpbf_dep = declare_dependency(link_with: libpbf, include_directories: '.')
