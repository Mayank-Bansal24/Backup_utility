project ('backup_util',
	 'cpp',
	  version : '0.1',
	  default_options : ['cpp_std=c++17'])
boost_dep = dependency('boost', modules : ['filesystem']) 

executable ('backup_util',
	    'main.cxx',
		'init.cxx',
		'dir_struct.cxx',
	     dependencies : boost_dep)


