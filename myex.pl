#!/usr/bin/perl

#---------------------------------------------------------------------------
# 12/31/2013 James Wang
#   - To build OpenSAF according to the config file
#   - To install the built OpenSAF package onto the PCREFs.
#----------------------------------------------------------------------------
use Env;
use Env qw(PATH HOME TERM);
use lib ("$HOME/myRepos/net-automation");
use IO::Handle;
use File::Path;
use POSIX ":sys_wait_h";
use Cwd;
use strict;

use PM::Time;
use PM::MyUtil;
use PM::Logger;

############################################################################
# Declare Default Global Variables in this section
############################################################################
$::MAIN = 1;
$::FAIL = -1;
$::SUCCESS = 0;
$::SUFFIX_CMD = ".CMD";
$::SUFFIX_DESP = ".description";

$::DEBUG_MODE = 0;

$::CFG_ROOT_DIR = "";

my $time = new PM::Time();
$::CURRENT_DATE = $time->getDateTag();
$::CURRENT_TIME = $time->getTimeTag();


my $toolname = basename($0);

our $topDir = getcwd();
my $logdir = "/tmp/logs";

`mkdir -p $logdir`;

$::LOG_FILE = "$logdir/$toolname".".log";
$::LOG_CMD_FILE = "$logdir/$toolname".".cmd";

%::ETC_CONFIG=();

############################################################################
# Main Execution Starts Here
############################################################################
our $topDir = getcwd();
my $status;

#To clean the previous log files under /tmp...
`rm -rf /tmp/$toolname*`;

&log (" ");
&log ("The current working dir is: $::topDir");
&log ("The log file: $::LOG_FILE");
    &log ("I am going to build the package.");
    &log (" ");

my $cfgfile = "/home/ejenwng/mybin/$toolname".".cfg";
use Getopt::Long;
    &GetOptions ("help" => \&get_help,
                 "config=s" => \$cfgfile,
                 "debug" => \$::DEBUG_MODE
                 );


our %TQ_CONFIG = &readConfig($cfgfile);
if (!%TQ_CONFIG){
    &log("Failed in Reading the configure file - please double check.");
    exit($::FAIL);
}

our $test_result_logfile = "";
my $cfile = (split(/\//, $cfgfile))[-1];
$::CFG_ROOT_DIR = $cfgfile;
$::CFG_ROOT_DIR =~ s/$cfile//;
$::CFG_ROOT_DIR = "." if ($::CFG_ROOT_DIR eq "");

$::TQ_CONFIG{CFG_ROOT_DIR} = $::CFG_ROOT_DIR;

&log (" ");
&log ("Now going to execute the commands in:");
&log ("  $cfgfile ... ... $::CFG_ROOT_DIR");
&log (" ");

foreach my $key (sort(keys %::TQ_CONFIG)) {
#    $::LOGGER->print(" key: $key \n");
    next if ($key !~ /^STEP_\d+$::SUFFIX_CMD/);
    my $cmd = &cfgReplaceVariables($::TQ_CONFIG{$key});

    my $prefix = $key;
    $prefix =~ s/$::SUFFIX_CMD//;
    my $desp = $cmd;
    my $key_desp = $prefix.$::SUFFIX_DESP;
    $desp = &cfgReplaceVariables(" $::TQ_CONFIG{$key_desp}") if ($::TQ_CONFIG{$key_desp});

    my $logfile = "$::LOG_FILE".".$key";
    &log ($prefix.": $desp - log file $logfile:");

    $::LOGGER->print("  To run the cmd: $cmd ... ...");

    &pause("Before run $cmd") if ($::DEBUG_MODE);

    $status = system ("$cmd > $logfile 2>&1");
    $::LOGGER->append(" Done with status $status \n");
    &log(" ");
    if (($status != 0) && ($desp !~ /continue_on_error/)){
        &log ("Something wrong in $prefix: $cmd - please double check.");
        &log ("The detail log: \n");
#       my $output = `cat $logfile`;
#       &log($output);
        &log (" $logfile\n");
        exit ($::FAIL);
    }

#    &pause("After $desp") if ($::DEBUG_MODE);

}

&log (" ");
&log (" Done Successfully! The detail log:");
&log ("      $::LOG_FILE".".STEP_<#step_no>.CMD");
&log (" ");


exit ($::SUCCESS);

#############################################
#Main Execution Ends Here
#############################################


sub fetchResultFile{
    my ($file, $cmd) = @_;
    my $ret = $::FAIL;

    my $index = 0;
    my $interval = 30;
    my $maxchecks = 120;
    my $maxtimer =  $maxchecks * $interval / 60;

    $::LOGGER->print (" To waiting for the test result: $file ... ...");

    while (($index < $maxchecks) && ($file) && !(-e $file)){
        $status = system("$cmd > $::LOG_FILE".".fetchfile_cli");
        if ($status != 0){
            &log ("Something wrong in $cmd - please double check - will try again.");
        }

        $index++;
        $::LOGGER->append ("!");
        sleep ($interval);
    }
    $::LOGGER->append ("Done.\n");

    if (-e $file){
        &log(" The result file returned: $file");
        $ret = $::SUCCESS;
    }else {
        &log ("The file of $file not returned after waiting for $maxtimer mins - please double check:");
    }
    return $ret;
}



###
 # To update configuration files under /etc/opensaf/
 #
 # Input : $dist_dir: the filename of the configuraion with full path
 # Output: %key     : the associated list contains the pair of key/value
###
sub updateNodeConfig{
    my ($dist_dir, $nodekey) = @_;

    $::LOGGER->print ("  To update node of $nodekey -> $::TQ_CONFIG{$nodekey} ... ... ");
    my $etc_dir = "$dist_dir/etc_$::TQ_CONFIG{$nodekey}";
    my $status = `mkdir -p $etc_dir`;
    $::ETC_CONFIG{$::TQ_CONFIG{$nodekey}} = $etc_dir;
    $status = `echo $::TQ_CONFIG{$nodekey} > $etc_dir/node_name`;
    my $slotid = PM::MyUtil::trim((split /-/, $nodekey)[-1]);
    my $type = PM::MyUtil::trim((split /-/, $nodekey)[0]);
    if ($type =~ /SC/) {
        $type = "controller";
    }else{
        $type = "payload";
    }
    $status = `echo $slotid > $etc_dir/slot_id`;
    $status = `echo $type > $etc_dir/node_type`;

    $status = `sed '/^export MDS_TRANSPORT=TIPC/ a ORE_ID=4711' $dist_dir/etc/opensaf/nid.conf >$etc_dir/nid.conf`;
    $status = `sed -i '/^export MDS_TRANSPORT=TIPC/ a TIPC_NET_ID=4711' $etc_dir/nid.conf`;
    $status = `sed -i '/^export MDS_TRANSPORT=TIPC/ a TIPC_ETH_IF=eth0' $etc_dir/nid.conf`;
    $status = `sed -i 's/^export OPENSAF_GROUP=opensaf/export OPENSAF_GROUP=root/g' $etc_dir/nid.conf`;
    $status = `sed -i 's/^export OPENSAF_USER=opensaf/export OPENSAF_USER=root/g' $etc_dir/nid.conf`;

    $status = `sed -i 's/$nodekey $nodekey/$nodekey $::TQ_CONFIG{$nodekey}/g' $dist_dir/usr/local/share/opensaf/immxml/nodes.cfg`;
    $status = `cd $etc_dir; cp $dist_dir/etc/opensaf/nodeinit.conf.$type .; ln -fs nodeinit.conf.$type nodeinit.conf`;

    $::LOGGER->append ("Done.\n");
}


###
 # To read a configuration file with format of <kye_string> = <value_string>
 #
 # Input : $cfgfile: the filename of the configuraion with full path
 # Output: %cfg    : the associated list contains the pair of key/value
###
sub readConfig{
    my ($cfgfile) = @_;
    my %cfg = ();
    if (!open (CONFIG, "<", $cfgfile)) {
        &log ("Could not open $cfgfile")
#        return $::FAIL;
    }

    my @details = <CONFIG>;
    close CONFIG;

    foreach my $line (@details) {
        $line = &PM::MyUtil::trim($line);
        # To skip the empty lines or comment lines (starts with "#")...
        next if ($line eq "" or $line =~ /^#/);
        my ($key, $value) = split /:=/, $line;
        $key = &PM::MyUtil::trim($key);
        $value = &PM::MyUtil::trim($value);

        next if $value eq "";
        $value =~ s/JENKINS_CURRENT_DIR/$::topDir/g;
        $value =~ s/CURRENT_WORK_DIR/$::topDir/g;
        $value =~ s/SSH_CMD/$cfg{SSH_CMD}/g if ($cfg{SSH_CMD});
        $value =~ s/CURRENT_DATE/$::CURRENT_DATE/g if ($::CURRENT_DATE);
        $value =~ s/CURRENT_TIME/$::CURRENT_TIME/g if ($::CURRENT_TIME);
        $cfg{$key} = $value;

     } # The end of the loop "foreach my $line (@details){}"
    return %cfg;
}# The end of the sub of readConfig().



############################################################################
# Description : Going to download the default configuration files via ftp
#
# Input       : none
#
# Output      : none
############################################################################

########################################################################
# Description : This function is used to print help messages of this tool.
#
# Input       : none
#
# Output      : none
########################################################################
sub get_help {

    use File::Basename;
    my $toolname = basename($0);
    print <<"EOF";
    Description:
    This tool is to build OpenSAF according to the given config file:
        - to build OpenSAF installing packages ready for deploy onto the PCREFs.
        - to build OpenSAF and install it locally for build the AMF apps or samples

    Usage:

        $toolname [--config <config_file> | --help]

        --config : To specify the configurations how to build/install OpenSAF.
                   The default is /home/ejenwng/mybin/cfg/$toolname .cfg.

        --help   : print this help

EOF
    exit $::SUCESS;
}

#
# To
#
sub forkAJob(){
    my ($sys_cmd, $build_dir, $install_dir) = @_;
    my $ret = 0;
#    &log(" Entering sub forkAJob()\n");
    if (my $pid = fork){ # In the main process
        my $statusFile = $::LOG_FILE;
        $statusFile =~ s/\.log/\.$pid\.status/;
        $::PROCESSID_CHILDREN->{$pid}{STATUS_FILE}= $statusFile;
        $ret = $pid;
        my $logFile = $statusFile;
        $logFile =~ s/\.status/\.log/;
 #       &log(" forAJob() :: Parent: Job Child, pid=$pid, was stored: Log File: $logFile, Status File: $statusFile");
    }
    else{ # In the child process
        ##############################
        $::LOG_FILE =~s/\.log/\.$$\.log/;
        my $logger = new PM::Logger($::LOG_FILE);

        $logger->println ("Now is going to execute: $sys_cmd");
sleep 10;
        my $status = system("$sys_cmd > null");

        my $statusFile = $::LOG_FILE;
        $statusFile =~ s/\.log/\.status/;
        open FID, ">$statusFile";
        print FID "$status";
        close FID;
        ##############################
        $logger->println ("The child process is done");
        exit 0;
    } # The end of 'if (my $pid = fork)else{}'

 #   &log(" Leaving sub forkAJob()\n");
    return $ret;
} # The end of sub forkAJob()

###
 # To wait for the termination of children process.
 #
 # Input : None
 # Return: None
###
sub waitChildrenToExit{
    my ($pidsChildren) = @_;
    my @pids = @{$pidsChildren};
    my @pids_exited = ();
    my $maxNoChild = 60;
    &log("waitChildrenToExit():: Waiting for chdilren to exit: @pids \n");
    my $pid_count = @pids;
    my $pid_exited_count = @pids_exited;
    no strict;

    while ($pid_count > $pid_exited_count){

        my $stiff = waitpid (-1, WNOHANG);
#       &log("waitChildren():: waitpid returns: $stiff \n");
        if ($stiff == -1) {
            # wierd value to get... how to handle?
            &log("waitChildrenToExit():: Retruned -1. Not good, try again.");
            $maxNoChild--;
            sleep 1;
            if ($maxNoChild <= 0){
                &log("Still no good after $maxNoChild tries. - It might mean the children are already exited. Anyway, let's cleanup.\n"              );
                foreach my $id (@pids){
                    &log(" $id cleaned.\n");
                    push(@pids_exited, $id);
                }
                last;
            }
        } elsif (WIFEXITED($?)) {
            # child exited, undo testbed and start new test
            &log("waitChildrenToExit():: child process $stiff exited. Good.\n");
            push(@pids_exited, $stiff);
            $pid_exited_count = @pids_exited;
        } else {
            # no child exiting...and most likely will return 0
#           &log("waitChildren():: no one is waiting or exited? $stiff.\n");
            sleep(3);
        }
    }
    &log("waitChildrenToExit():: All chdilren exited: @pids_exited. Very Good!\n");
    return \@pids_exited;
}


###
 # To handle the CHLD signal received when the children process terminated.
 # It will wait for the children to prevent from being zombias.
 #
###

###
 # To log the messages using the global variable of $::LOGGER
 #
 # Input : $str: the message will be logged.
 # Return: None
sub log {
    my ($message)=@_;
    if (!$::LOGGER){
        $::LOGGER=new PM::Logger("STDOUT"); #Logger($::LOG_FILE);
    }
    $::LOGGER->println($message);
}



sub pause{
    my ($msg) = @_;
    print ("\n\nDEBUG_PAUSE:: $msg\n") if ($msg);
    print ("\n\nDEBUG_PAUSE:: Press any key to continue ... ...");
    $::INPUT=<STDIN>;
}


#
# Replace all $::TQ_CONFIG{...} in the the cmd line
#
sub cfgReplaceVariables {
    my ($cmd) = @_;
    my $ret = $cmd;
    while ($ret =~ /\$::TQ_CONFIG\{.*\}/) {
        $ret =~ s/(\$::TQ_CONFIG\{[^=\{]*\})/$1/eeg;
    }

    if (($ret =~ /^MYEX_UPDATE_FILE.*/) || ($ret =~ /"MYEX_UPDATE_FILE.*/)) {
#        print "\n cfgReplaceVariables():: cmd: $ret\n";

        my ($func, $file, $id, $update, $area, $trailing) = split (/::/, $ret);
        if ($area) {
            my $inssh = ($ret =~ /"MYEX_UPDATE_FILE/);
            my $cmd =  "if sudo grep -q '$id' $file; then echo 'It has been configured - skipped this step'; else sudo sed -i '$area a\\$update' $file; fi";
            $ret =~ s/MYEX_UPDATE_FILE.*/$cmd/;
            if ($inssh) {
                $ret .= '"';
            }
        } else {
            &log("Something wrong with the func call in the cfg file: $ret\n");
            exit($::FAIL);
        }
    }

    return $ret;
}

