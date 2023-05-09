project ('backup_util',
	 'cpp',
	  version : '0.1',
	  default_options : ['cpp_std=c++17'])
	  
boost_dep = dependency('boost', modules : ['filesystem']) 
libarch_dep = dependency('libarchive')

executable ('backup_util',
	    'main.cxx',
		'init.cxx',
		'file.cxx',
		'dir_struct.cxx',
		'log.cxx',
		'json.hxx',
		dependencies : [boost_dep, libarch_dep],
		cpp_args :['-g'])

	    


