#!/usr/bin/perl

#---------------------------------------------------------------------------
# 05/12/2017 James Wang
#   - To build project based on the configuration file provided
#----------------------------------------------------------------------------
use Env;
use Env qw(PATH HOME USER UID GROUPS);
use Cwd qw(abs_path getcwd);
use File::Basename;
use lib(dirname(abs_path($0)));
use FileHandle;
use File::Path;
use File::Basename;
use POSIX ":sys_wait_h";
use strict;
use Time::HiRes;
use Getopt::Long;
require lib;

use PM::Time;
use PM::MyUtil;
use PM::Logger;

############################################################################
# Declare Default Global Variables in this section
############################################################################
$::FAIL = -1;
$::SUCCESS = 0;

$::GROUP_KEY_CURRENT = "CURRENT_GROUP"; # Indicate current group
$::GROUP_KEY_SURFIX = "GRP"; # Indicate the group for the included cfg file
$::GROUP_KEY_DEFAULT = $::GROUP_KEY_SURFIX . '01'; # The default group
$::GROUP_KEY_CFG = "CFG";
$::GROUP_KEY_DESP = "description";

$::GROUP_KEY_REPORT = "group_report";

$::STEP_SURFIX = "STEP_"; # exmaple: STEP_001.CMD := foo
$::STEP_DESP_INTERACTIVE ="- interactive";
$::STEP_KEY_CMD = "CMD";
$::STEP_KEY_PACKAGE = "package";
$::STEP_KEY_NAME = "name";
$::STEP_KEY_DESP = "description";
$::STEP_KEY_STATUS = "STATUS";
$::STEP_KEY_RETRIES = "RETRIES";
$::STEP_KEY_LOGFILE = "LOGFILE";
$::STEP_KEY_LOGDIR = "LOGDIR";
$::STEP_KEY_DURING = "DURING";
$::STEP_KEY_REPORTING = "REPORTING";
$::STEP_KEY_POST_ACTION = "POST_ACTION";
$::STEP_KEY_CONTINUE_ON_FAIL = "continue_on_fail";
$::STEP_KEY_REPORT_DIR = "REPORT_DIR";
$::STEP_KEY_REPORT_FILE = "REPORT_FILE";
$::STEP_KEY_NAME_IF_PREFIXED = "name_if_tc";
$::REPORT_DIR = "test_reports";

$::DEBUG_MODE = 0;
$::LOG_DETAILS = 0;

my $time = new PM::Time();
$::CURRENT_DATE = $time->getDateTag();
$::CURRENT_TIME = $time->getTimeTag();


my $toolname = basename($0);

our $LOGDIR = "/tmp/logs/$::CURRENT_DATE" . "_$::CURRENT_TIME";
`mkdir -p $::LOGDIR; sudo chmod -R a+rwx /tmp/logs`; # create the log parent dir if not exists yet

$::LOG_FILE = "$::LOGDIR/$toolname".".log";
$::LOG_CMD_FILE = "$::LOGDIR/$toolname".".cmd";

############################################################################
# Main Execution Starts Here
############################################################################
$::CURRENT_WORK_DIR = getcwd();
$::TOOL_ROOT_DIR = dirname(abs_path($0));
my $status;

#To clean the previous log files under /tmp...
`rm -rf /tmp/$toolname*`;

my $cfgfile = "$::CURRENT_WORK_DIR/" . "$toolname".".cfg";
    &GetOptions ("help" => \&get_help,
                 "config=s" => \$cfgfile,
                 "debug" => \$::DEBUG_MODE,
                 "details" => \$::LOG_DETAILS
                 );

&log (" ");
&log ("The current working dir is: $::CURRENT_WORK_DIR");
&log ("The log file: $::LOG_FILE");

my $cfile = (split(/\//, $cfgfile))[-1];
$::CFG_ROOT_DIR = $cfgfile;
$::CFG_ROOT_DIR =~ s/$cfile//;
$::CFG_ROOT_DIR = "." if ($::CFG_ROOT_DIR eq "");
&log ("The configure file: $cfile");

&log ("I am going to build the project.");

my %mainCfg = &readCfgFile($cfgfile);
my %groups = &processGroups(\%mainCfg);

if (!%mainCfg){
    &log("Failed in Reading the configure file - please double check.");
    exit($::FAIL);
}

&log (" ");
&log ("Now going to execute the commands in:");
&log ("  $cfgfile ... ... $::CFG_ROOT_DIR");
&log (" ");

foreach my $group_key (sort(keys %groups)) {
    my $cfg = $groups{$group_key}{$::GROUP_KEY_CFG};
    my $cfg_desp = $groups{$group_key}{$::GROUP_KEY_DESP};
    my $grp_report = ( $groups{$group_key}{$::GROUP_KEY_REPORT} ) ?
         $groups{$group_key}{$::GROUP_KEY_REPORT} :
        "$::CURRENT_WORK_DIR/$group_key" . "_report.xml";
    &log("  Now process group: $group_key: $cfg_desp : $grp_report ...");

    foreach my $key (sort(keys %{$cfg})) {
        #    $::LOGGER->print(" key: $key \n");
        next if ($key !~ /^$::STEP_SURFIX\d+$/);
        my $cmd = &cfgReplaceVariables($cfg->{$key}->{$::STEP_KEY_CMD}, $cfg);

        my $desp = ($cfg->{$key}->{$::STEP_KEY_DESP}) ?
           &cfgReplaceVariables(" $cfg->{$key}->{$::STEP_KEY_DESP}", $cfg) :
           $cmd;

        my $logfile = "$::LOG_FILE" . ".$group_key" . ".$key";
        &log("$group_key"."_$key: $desp - log file $logfile ...");
        $cfg->{$key}->{$::STEP_KEY_LOGFILE} = $logfile;

        &log("  To run the cmd: $cmd ... ...");
        $cmd = $cmd."; pwd" if ($::LOG_DETAILS ||
                                $desp =~ /$::STEP_DESP_INTERACTIVE/);
        &pause("Before run $cmd") if ($::DEBUG_MODE);
        my $status = &runStepCmd($cfg, $key, $cmd, $logfile);

        if ($status != 0) {
            if ($cfg->{$key}->{$::STEP_KEY_CONTINUE_ON_FAIL}) {
                for (my $in = 0; $in < $cfg->{$key}->{$::STEP_KEY_RETRIES};
                     $in ++) {
                    &log("  Something wrong in $group_key" .
                         "_$key: $cmd");
                    &log("  The detail log: \n");
                    &log("    $logfile\n");
                    &log("  Will Rerun: $cmd ...\n");
                    $status = &runStepCmd($cfg, $key, $cmd, $logfile);
                    last if ($status == 0);
                }
            } else {
                &log("Something wrong in $group_key" .
                     "_$key: $cmd - please double check.");
                &log("The detail log: \n");
                &log("  $logfile\n");
                exit ($::FAIL);
            }
        }
        my $status_str = ($status == 0) ? "=== SUCCESS ===" : "!!! FAIL !!!";
        &log("  - Done with $status_str");

        if ($cfg->{$key}->{$::STEP_KEY_REPORTING}) {
            &generateTestReport($cfg->{$key}, $grp_report);
        }
        &log("  - Log file: $logfile");
        &log(" ");

    } # The end of foreach my $key (sort(keys %::TQ_CONFIG))

} # The end of foreach my $group

&log (" ");
&log (" All Done Successfully! The detail log:");
&log ("      $::LOG_FILE".".$::STEP_SURFIX<#step_no>.CMD");
&log (" ");

exit ($::SUCCESS);

#############################################
#      Main Execution Ends Here
#############################################

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
        &log ("Could not open $cfgfile - please double check then try again.");
        exit $::FAIL;
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
        $value =~ s/JENKINS_CURRENT_DIR/$::CURRENT_WORK_DIR/g;
        $value =~ s/CURRENT_WORK_DIR/$::CURRENT_WORK_DIR/g;
        $value =~ s/CFG_ROOT_DIR/$::CFG_ROOT_DIR/g;
        $value =~ s/TOOL_ROOT_DIR/$::TOOL_ROOT_DIR/g;
        $value =~ s/SSH_CMD/$cfg{SSH_CMD}/g if ($cfg{SSH_CMD});
        $value =~ s/CURRENT_DATE/$::CURRENT_DATE/g if ($::CURRENT_DATE);
        $value =~ s/CURRENT_TIME/$::CURRENT_TIME/g if ($::CURRENT_TIME);
        my @keys = split /\./, $key;
        my $keys_count = @keys;
        if ($keys_count > 1){
            $cfg{$keys[0]}{$keys[-1]} = $value;
        } else {
            $cfg{$key} = $value;
        }
    } # The end of the loop "foreach my $line (@details){}"
    return %cfg;
}# The end of the sub of readConfig().

########################################################################
# Description : This function is used to Read cfg files, .cfg or .xml
#
# Input       : $cfgfile the cfg file, .cfg or .xml
#
# Output      : %cfg the configuration
########################################################################
sub readCfgFile($cfg) {
    my ($cfgfile) = @_;
    my %cfg = ();
    if ($cfgfile =~ /xml$/) {
        %cfg = &getCfgFromXml($cfgfile);
    }else {
        %cfg = &readConfig($cfgfile);
    }
    return %cfg;
}

########################################################################
# Description : This function is used to process the action groups
#
# Input       : the original cfg map variable
#
# Output      : the action group map variable
########################################################################
sub processGroups() {
    my ($cfg) = @_;
    my %group = ();

    # Process the current group for the main cfg file
    if ( defined $cfg->{$::GROUP_KEY_CURRENT} ) {
        $group{$cfg->{$::GROUP_KEY_CURRENT}->{$::GROUP_KEY_CFG}}{$::GROUP_KEY_CFG} = $cfg;
        $group{$cfg->{$::GROUP_KEY_CURRENT}->{$::GROUP_KEY_CFG}}{$::GROUP_KEY_DESP} = $cfg->{$::GROUP_KEY_CURRENT}->{$::GROUP_KEY_DESP};
    } else {
        $group{$::GROUP_KEY_DEFAULT}{$::GROUP_KEY_CFG} = $cfg;
        $group{$::GROUP_KEY_DEFAULT}{$::GROUP_KEY_DESP} = "Main Configuration";
    }

    # Now to process the included cfg files
    foreach my $key (sort(keys %{$cfg})) {
        #    $::LOGGER->print(" key: $key \n");
        next if ($key !~ /^$::GROUP_KEY_SURFIX\d+$/);

        my $grpFile = &cfgReplaceVariables($cfg->{$key}->{$::GROUP_KEY_CFG},
                                           $cfg);
        my $grpDesp = ($cfg->{$key}->{$::STEP_KEY_DESP}) ?
            &cfgReplaceVariables(" $cfg->{$key}->{$::GROUP_KEY_DESP}", $cfg) :
            $grpFile;

        $grpFile = &PM::MyUtil::trim(`ls $grpFile`); # To-be-debug!!!
        &log ("  Going to include the group $key: $grpFile ...");
        my %grp_cfg = &readCfgFile($grpFile);
        $group{$key}{$::GROUP_KEY_CFG} = \%grp_cfg;
        $group{$key}{$::GROUP_KEY_DESP} = $grpDesp;
        $group{$key}{$::GROUP_KEY_REPORT} =
            ( $cfg->{key}->{$::GROUP_KEY_REPORT} ) ?
            $cfg->{key}->{$::GROUP_KEY_REPORT} :
            "$::CURRENT_WORK_DIR/$key" . "_report.xml";
        system ("rm -rf $group{$key}{$::GROUP_KEY_REPORT}"); # clean it up
    }

    return %group;
}

########################################################################
# Description : This function is used to execute the step command
#
# Input       : the original cfg map variable, key, cmd and logfile
#
# Output      : executing status, non-0 for FAIL or 0 or PASS
########################################################################
sub runStepCmd {
    my ($cfg, $key, $cmd, $logfile) = @_;
    my $startTime = time();
    my $status = system ($cmd . ' 1>>' . $logfile . ' 2>&1');
    my $endTime = time();
    $cfg->{$key}->{$::STEP_KEY_STATUS} = $status;
    my $during = (($endTime - $startTime) == 0) ? 0.1 : $endTime - $startTime;
    $cfg->{$key}->{$::STEP_KEY_DURING} = $during;
    return $status;
}
########################################################################
# Description : This function is used to print help messages of this tool.
#
# Input       : none
#
# Output      : none
########################################################################
sub get_help {

    my $toolname = basename($0);
    print <<"EOF";

    Description:

    This tool is to execute a batch of cmds according to the given config file:
        - to run the cmd steps in the a file - format in the sample cfg files
        - to run the testcases in the a file - format in the sample xml files

    Usage:

        $toolname [--config <config_file> | --help]

        --config : To specify the configurations how to build/install actions.
                   Default is $::CURRENT_WORK_DIR/$toolname.cfg.
            NOTE :
                   If the configure file is a xml, it would be parsed as
                   testing suite file, which should include <TestCase> tag
                   under <ProductRegression>

        --help   : print this help
        --debug  : Pause before each command
        --details: Log details for each command to screen (;pwd appended)

EOF
    exit $::SUCESS;
}

###
 # To create a new child process
###
sub forkAJob(){
    my ($sys_cmd, $build_dir, $install_dir) = @_;
    my $ret = 0;
    if (my $pid = fork){ # In the main process
        my $statusFile = $::LOG_FILE;
        $statusFile =~ s/\.log/\.$pid\.status/;
        $::PROCESSID_CHILDREN->{$pid}{STATUS_FILE}= $statusFile;
        $ret = $pid;
        my $logFile = $statusFile;
        $logFile =~ s/\.status/\.log/;
    } else{ # In the child process
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
    return $ret;
}

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
        #  &log("waitChildren():: waitpid returns: $stiff \n");
        if ($stiff == -1) {
            # wierd value to get... how to handle?
            &log("waitChildrenToExit():: Retruned -1. Not good, try again.");
            $maxNoChild--;
            sleep 1;
            if ($maxNoChild <= 0){
                &log("Still no good after $maxNoChild tries." .
                     " - It might mean the children are already exited." .
                     " Anyway, let's cleanup.\n"              );
                foreach my $id (@pids){
                    &log(" $id cleaned.\n");
                    push(@pids_exited, $id);
                }
                last;
            }
        } elsif (WIFEXITED($?)) {
            # child exited, undo testbed and start new test
            &log("waitChildrenToExit():: child process $stiff exits - Good.\n");
            push(@pids_exited, $stiff);
            $pid_exited_count = @pids_exited;
        } else {
            # no child exiting...and most likely will return 0
            # &log("waitChildren():: no one is waiting or exited? $stiff.\n");
            sleep(3);
        }
    }
    &log("waitChildrenToExit():: All chdilren exited: @pids_exited - Great!\n");
    return \@pids_exited;
}

###
 # To log the messages using the global variable of $::LOGGER
 #
 # Input : $str: the message will be logged.
 # Return: None
###
sub log {
    my ($message)=@_;
    if (!$::LOGGER){
        $::LOGGER=new PM::Logger("STDOUT");
    }
    $::LOGGER->println($message);
}


###
 # Util api for debug purpose
 #
 # Input : None
 # Return: None
###
sub pause{
    my ($msg) = @_;
    print ("\n\nDEBUG_PAUSE:: $msg\n") if ($msg);
    print ("\n\nDEBUG_PAUSE:: Press ENTER to continue ... ...");
    $::INPUT=<STDIN>;
}


###
 # Replace all $::TQ_CONFIG{...} in the the cmd line
 #
 # Input :
 #     $cmd: the cmd string needs to be replace with variables
 #     $cfg: the configuraiton map of the same group
 # Return: the replaced cmd string
###
sub cfgReplaceVariables {
    my ($cmd, $cfg) = @_;
    my $ret = $cmd;
    while ($ret =~ /\$::TQ_CONFIG\{.*\}/) {
        $ret =~ s/\$::TQ_CONFIG\{([^=\{]*)\}/$cfg->{$1}/;
    }

    # Updating files based on tailing patten
    if (($ret =~ /^MYEX_UPDATE_FILE.*/) || ($ret =~ /"MYEX_UPDATE_FILE.*/)) {
        my ($func, $file, $id, $update, $area, $trailing) = split (/::/, $ret);
        if ($area) {
            my $cmd =  "if sudo grep -q '$id' $file; " .
                "then echo 'It has been configured - skipped this step'; " .
                "else sudo sed -i '$area a\\$update' $file; fi";
            $ret =~ s/MYEX_UPDATE_FILE.*/$cmd/;
            $ret .= '"' if ($ret =~ /"MYEX_UPDATE_FILE/); #in ssh session
        } else {
            &log("Something wrong with the func call in the cfg file: $ret\n");
            exit($::FAIL);
        }
    }

    # Watchdog the long run process
    if (($ret =~ /^MYEX_WATCHDOG.*/) || ($ret =~ /"MYEX_WATCHDOG.*/)) {
        my ($func, $pattern, $limit, $recover_cmd) = split (/::/, $ret);
        if ($pattern) {
            my $ps =  "ps -eo pid,ppid,etime,cmd | grep $pattern";
            my @out = `$ps`;
            my $index = 0;
            my %ps;
            my @ps_parent = ();

            $limit = 1800 if (! defined $limit);
            foreach my $line (@out) {
                $line = &PM::MyUtil::trim($line);
                my ($pid, $ppid, $etime, $cmd) = split (/\s+/, $line);
                if ($etime) {
                    $ps{$index}->{"PID"}=$pid;
                    $ps{$index}->{"PPID"}=$ppid;
                    $ps{$index}->{"CMD"} = $cmd;
                    $ps{$index}->{"ETIME"} = $etime;
                    push (@ps_parent, $ppid);
                    $index++;
                } else {
                    &log("Something wrong in the cfg file: $ret\n");
                    exit($::FAIL);
                }
            }
            my $cmd =  "{ ";
            for (my $i = 0; $i < $index; $i++) {
                my $pid = $ps{$i}->{'PID'};
                my $ppid = $ps{$i}->{'PPID'};
                my $etime = $ps{$i}->{"ETIME"};
                $cmd .= " echo $i: pid: $pid, ppid: $ppid, etime: $etime; ";
                if ( grep ( /^$pid$/, @ps_parent)) {
                    $cmd .= "echo $pid is not leaf so skip; ";
                } else {
                    my @t = reverse split (/[:-]/, $etime);
                    my $ts = $t[0] + $t[1] * 60 + $t[2] * 3600 + $t[3] * 86400;
                    if ($ts > $limit) {
                        $cmd .= " echo $pid over limit of $limit; kill -9 $pid; sleep 1; ";
                        $cmd .= " $recover_cmd; sleep 1; " if ($recover_cmd);
                    } else {
                        $cmd .= " echo $pid is not over limit of $limit: $ts seconds only; ";
                    }
                }
            }
            $cmd .= " } ";
            $ret =~ s/MYEX_WATCHDOG.*/$cmd/;
        }
    }

    return $ret;
}

###
 # Parse XML File
 #
 # Input : The xml file full path
 # Return: The file handle for the xml file
###
sub ParseXmlFile {
    lib->import('XML::Simple');
    lib->import('XML::Writer');
    lib->import('XML::Merge');
    eval "use XML::Simple"; die $@ if $@;
    eval "use XML::Writer"; die $@ if $@;
    eval "use XML::Merge"; die $@ if $@;

    my ( $xmlFilePath) = @_;
    my $configHandle = undef;
    my $xmlParser = new XML::Simple;
    my $xmlStr = `envsubst < $xmlFilePath`;
    if ( $xmlStr ) {
        eval { $configHandle = $xmlParser->XMLin($xmlStr); };
        &log("Warning: Failed to parse '$xmlFilePath'") if $@;
    } else {
        &log("Config File $xmlFilePath not found - please double check." );
        exit ($::FAIL);
    }
    return $configHandle;
}

###
 # Reading the XMF files, the test suite file
 #
 # Input : The xml file full path
 # Return: The cfg structure contains the data from the xml file
###
sub getCfgFromXml {
    my ($cfgFile) = @_;
    my %cfg = ();
    my $suites = ParseXmlFile($cfgFile);

    # Creat the report dir and prepare for test results
    my $reportDir = (exists ($suites->{TestingResultDir})) ?
        $suites->{TestingResultDir} : $::REPORT_DIR;
    $reportDir = "$::CURRENT_WORK_DIR/$reportDir";
    system("mkdir -p $reportDir");

    my $name_if_tc = (exists ($suites->{Parameters}) &&
                      exists ($suites->{Parameters}->{TcNameIfPrefixed})) ?
        $suites->{Parameters}->{TcNameIfPrefixed} : 0;

    my $index = 1;
    my $suiteName = (exists ($suites->{Parameters})) ?
                     $suites->{Parameters}->{WhichSuite} : "ProductRegression";

    if (exists ($suites->{$suiteName})) {
        #print "Found ProductionRegression tag\n";
        my $scripts = $suites->{$suiteName};

        my @tss =(ref($scripts->{TestCase}) eq 'ARRAY') ?
            @{$scripts->{TestCase}} : ($scripts->{TestCase});

        my $runGcov = (exists ($suites->{$suiteName}->{runGcov})) ?
            $suites->{$suiteName}->{runGcov} : 0;

        no strict;
        my $qa_topdir = (defined ${TNT_TCNG_QA_TOPDIR}) ?
            ${TNT_TCNG_QA_TOPDIR} : "${HOME}/workspace/TCNG_QA";
        my $ws_topdir = (defined ${TNT_WORKSPACE_TOPDIR}) ?
            ${TNT_WORKSPACE_TOPDIR} : "${HOME}/workspace/TCNG_Software";
        my $mid_topdir = (defined ${TNT_EXTERNAL_REPO_MIDDLEWARE}) ?
            ${TNT_EXTERNAL_REPO_MIDDLEWARE} :
            "$ws_topdir/external_repos/TCNG_Middleware";
        use strict;

        foreach my $ts (@tss) {
            #print "Found TestCase: tc: $ts, name: $ts->{scriptName} \n";
            my $pre = ($index < 10) ? $::STEP_SURFIX . "00" : ($index < 100) ?
                $::STEP_SURFIX . "0" : $::STEP_SURFIX;

            my $step = $pre.$index;
            my $scriptName = $ts->{scriptName};
            $scriptName =~ s/\./_/g;
            my $name_only = (split (/\./, $ts->{scriptName}))[-1];
            $ts->{scriptName} =~ s/\./\//g;
            my $cmd = "echo  perl $ts->{scriptName}";

            if ($suiteName =~ /Unit/) {
                #
                # For unit test execuation:
                #     The log file should contains the test result
                #
                my $setup_cmd= 'export TNT_PLATFORM_NAME=tcng_at91cap9_e132mb;'.
                    " { . $ws_topdir/integration/build.env 2>&1 ";

                my $base_dir =  ("TCNG_Software" eq $ts->{repo}) ?
                    "$ws_topdir/" : "$mid_topdir/";

		my $build_cmd = ${setup_cmd} .
                    ' && cd ' . ${base_dir} . $ts->{sub_dir} .
                    ' && make -f ' . $ts->{makefile} . ' clean 2>&1'.
                    ' && make -f ' .$ts->{makefile} . ' BUILD_UNIT_TESTS 2>&1' .
                    ' && make -f ' . $ts->{makefile} . " RUN_UNIT_TESTS; }";
                $cmd = $build_cmd;

                # Handle the post action of each step
                my $gcov_data_root = (exists($ts->{gcovDataRoot})) ?
                    $ts->{gcovDataRoot} : "external_repos";
                my $gcov_cmd = "gcovr -r . $gcov_data_root" . " --xml-pretty" .
                    " > $reportDir/$scriptName" . "_gcov.xml";
                my $report_cmd =  ${setup_cmd} .
                    ' && cd ' . ${base_dir} . $ts->{sub_dir} .
                    ' && make -f ' . $ts->{makefile} . " REPORT; }";
                $cfg{$step}{$::STEP_KEY_POST_ACTION} = ($runGcov) ?
                    $gcov_cmd : $report_cmd;
            } else {
                $cfg{$step}{$::STEP_KEY_LOGDIR} =
                    "$qa_topdir/testcases/" . "$ts->{scriptName}" .
                    "/results";
                $ts->{scriptName} = $ts->{scriptName} . "/$name_only" . ".pl";
                $cmd = "cd $qa_topdir; perl testcases/$ts->{scriptName}";
            }

            $cfg{$step}{$::STEP_KEY_RETRIES} = (defined $ts->{retries}) ?
                $ts->{retries} : 1;
            $cfg{$step}{$::STEP_KEY_REPORTING} = 1;
            $cfg{$step}{$::STEP_KEY_PACKAGE} = $suiteName;
            $cfg{$step}{$::STEP_KEY_NAME} = $step . "_$scriptName";
            $cfg{$step}{$::STEP_KEY_NAME} =~ s/^$::STEP_SURFIX/TC_/;
            $cfg{$step}{$::STEP_KEY_CMD} = $cmd;
            $cfg{$step}{$::STEP_KEY_DESP} = "Running $ts->{scriptName}";
            $cfg{$step}{$::STEP_KEY_REPORT_DIR} = $reportDir;
            $cfg{$step}{$::STEP_KEY_CONTINUE_ON_FAIL} = 1;
            $cfg{$step}{$::STEP_KEY_REPORT_FILE} =
                "$reportDir/$cfg{$step}{$::STEP_KEY_NAME}" . ".xml";
            $cfg{$step}{$::STEP_KEY_NAME_IF_PREFIXED} = $name_if_tc;

            $index++;
        }
    }
    return %cfg;
}


###
 # To generate test report in JUnit format
###
sub generateTestReport {

    lib->import('Archive::Zip');
    lib->import('Encode');
    eval "use Archive::Zip"; die $@ if $@;
    eval "use Encode qw(decode encode)"; die $@ if $@;

    my ($step, $grp_report) = @_;
    my $run_status = $step->{$::STEP_KEY_STATUS};

    #
    # Retrieve the testcase name, based on the format in
    #     the description text generated in sub getCfgFromXml()
    #
    my $tcname = $step->{$::STEP_KEY_NAME};

    my @lines = ();
    if (!open (TESTLOG, "<", $step->{$::STEP_KEY_LOGFILE})) {
        &log ("Could not open $step->{$::STEP_KEY_LOGFILE}");
    } else {
        @lines = <TESTLOG>;
        close (TESTLOG);
    }

    # Parse the test log
    my $step_count = 1;
    my $test_count = 1;
    my $fails = ($run_status == 0)? 0 : 1;
    my $passes = ($run_status == 0)? 1 : 0;

    my $log_details = "";
    foreach my $line (@lines) {
        if ($line =~ /^\d+\.\.(\d+)\.*$/) {
            #the last line contains total checks
            $step_count = $1;
        }
        $line =~ s/[\x00-\x08\x0B-\x0C\x0E-\x1F]//g;
        $log_details .= $line;
    }

    my $stMessage = "Failures: Details see the full system output.";
    # Upload the testing logs if configured
    if ($step->{$::STEP_KEY_LOGDIR}) {
        my @list = `ls -t $step->{$::STEP_KEY_LOGDIR}`;
        my $log = $list[0];
        $stMessage = "Debug logs: $log saved under testlog $step->{$::STEP_KEY_REPORT_DIR}";
        $log = "$step->{$::STEP_KEY_LOGDIR}/$log";
        $log = &PM::MyUtil::trim($log);
        my $cmd = "cp $log $step->{$::STEP_KEY_REPORT_DIR}/";
        system($cmd);

        #read the conosle log from the zip file
        my $zipobj = Archive::Zip->new();
        if ($zipobj && !($zipobj->read($log))) {
            my @file_objs = $zipobj->members();
            my $debug = $zipobj->memberNamed("debugtty.log");
            my $logConsole = ($debug)? $debug->contents() : "";
            $logConsole =~ s/[\x00-\x08\x0B-\x0C\x0E-\x1F]//g;
            $log_details .= "\n\nConsole output:\n\n$logConsole";
        } else {
            &log(  "Error in retrieving console log in $log");
        }
    }
    $log_details = encode("utf8", $log_details);

    my $skips = $test_count - $fails - $passes;

    # Now to generate the report in the xml file
    my $report = $step->{$::STEP_KEY_REPORT_FILE};
    &log( "  To write test report into $report ..." );

    # Remove the prefix in the testcase name inside of the xml file
    $tcname =~ s/^TC_\d+_// if ($step->{$::STEP_KEY_NAME_IF_PREFIXED} == 0);

    my $fh = FileHandle->new( $report, "w" );

    my $writer = XML::Writer->new(OUTPUT => $fh, DATA_MODE => 1,
                                  DATA_INDENT => 2);

    $writer->xmlDecl('UTF-8');
    $writer->startTag('testsuites', name=>'TCNG TestSuites');
    $writer->startTag('testsuite', name => 'Single Testcase TestSuite',
                      tests => $step_count,
                      package =>  $step->{$::STEP_KEY_PACKAGE},
                      failures => $fails, skipped => $skips);

    my $status = ($run_status == 0)? "PASS" : "FAIL";
    $writer->startTag('testcase', name => $tcname, status => $status,
                      classname => $step->{$::STEP_KEY_PACKAGE}. '.' . $tcname,
                      time => $step->{$::STEP_KEY_DURING});

    $writer->dataElement('system-out' => $log_details);
    if ($run_status != 0) {
        $writer->startTag('failure', message => "Error Message");
        $writer->characters($stMessage);
        $writer->endTag('failure');
    }

    $writer->endTag('testcase');

    $writer->endTag('testsuite');
    $writer->endTag('testsuites');

    my $xml = $writer->end();

    #
    # Now merge the report
    # To-do: the merged result will be too big if too many TCs?
    #
#    if ( -f $grp_report ) {
        # Merge this report to the group one
#        my $main = XML::Merge->new('filename' => $grp_report);
#        $main->merge('filename' => $report);
#        $main->write();
#        system ("rm -rf $report");
#    } else {
        # Rename this report to be group one
#        system ("mv $report $grp_report");
#    }

    &log ("  - Done.");

    if (defined $step->{$::STEP_KEY_POST_ACTION})  {
        my $pcmd = $step->{$::STEP_KEY_POST_ACTION};
        &log ("  Post Action: $pcmd");
        my $plogfile = $step->{$::STEP_KEY_LOGFILE} . "_post_action";
        system ($pcmd . ' 1>' . $plogfile . ' 2>&1');
    }
}

