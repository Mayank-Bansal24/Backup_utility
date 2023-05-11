project ('backup_util',
	 'cpp',
	  version : '0.1',
	  default_options : ['cpp_std=c++17'])
	  
boost_dep = dependency('boost', modules : ['filesystem']) 
libarch_dep = dependency('libarchive')

meson.add_install_script('./install_.sh')

install_man('./backup_util.1')
executable ('backup_util',
	    'main.cxx',
		'init.cxx',
		'file.cxx',
		'dir_struct.cxx',
		'log.cxx',
		'json.hxx',
		dependencies : [boost_dep, libarch_dep],
		cpp_args :['-g'],
		install: true)
	    


