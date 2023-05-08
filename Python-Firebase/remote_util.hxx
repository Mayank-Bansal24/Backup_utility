#pragma once

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <filesystem>
#include "remote_util.hxx"

using namespace std;

int remoteutil(int args,char* argv[]);