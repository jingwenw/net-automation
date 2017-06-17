# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi

# set PATH so it includes user's private bin directories
PATH="$HOME/myRepos/net-automation:/sbin:$HOME/bin:$HOME/.local/bin:$PATH"
################ TCNG PROFILE DEFINES #########################
export TNT_TCNG_BUILD_TOOLS_TOPDIR="$HOME/workspace/TCNG_Build_Tools"
export TNT_TCNG_QA_TOPDIR="$HOME/workspace/TCNG_QA"
export TNT_WORKSPACE_TOPDIR="/var/jenkins/TCNG_Repos_Root"
export TNT_EXTERNAL_REPO_MIDDLEWARE="${TNT_WORKSPACE_TOPDIR}/external_repos/TCNG_Middleware"
export TNT_EXTERNAL_REPO_TUNETSYSSHARED="${TNT_WORKSPACE_TOPDIR}/external_repos/TUNetSysShared"
export TNT_EXTERNAL_REPO_ASIC="${TNT_WORKSPACE_TOPDIR}/external_repos/TCNG_ASIC"

export PERL5LIB="${TNT_TCNG_QA_TOPDIR}/framework/perlapi:${TNT_EXTERNAL_REPO_TUNETSYSSHARED}/perllib"

