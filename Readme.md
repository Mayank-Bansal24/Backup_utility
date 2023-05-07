# Why this Project? 
In the modern era, the amount of data for each user is too large and is growing significantly cumbersome for users. Manual monitoring is not possible to protect files. so Automatically storing that copy is an essential human need. Backup what matters User data is very important. If data is lost from the user's system, the user can recover it Always from backup. So we need a utility that can serve User-friendly backup and restore with a user-friendly interface, so even on the go Can be used in non-programming backgrounds.

# About the Project: 
In the end, we will develop a tool that provides you with the services you want with all the options for protecting your data. Also, the tool is very intuitive since we provide the user with the necessary options so that he can customize the backup process according to his needs. Everything is going very well. This tool can be run on any Linux machine with some pre-existing dependencies, which we also provide as a bash script for initial execution. Whether you back up your data locally or remotely is entirely up to you.Our tool can be beneficial for Linux users, which allows them to save their important files locally or remotely for data recovery in case of system crash (which is common for Linux users who love to program).

# Dependencies used to build this project

* **Dependencies**:
	1) Meson
	1) Boost
	1) Ninja

 ---
 
* **Steps to build the project**: 
	1) First install all the required dependencies.

		1.For installing Meson: `sudo pacman -S meson`

		2.For installing Boost: `sudo pacman -S boost`

		3.For installing Ninja: `sudo apt-get install ninja-build`

	1) mkdir build
	1) meson build
	1) cd ./build
	1) ninja
	1) ./backup_util
 
