#!/usr/bin/perl

#---------------------------------------------------------------------------
# 12/31/2013 James Wang
#   - To build OpenSAF according to the config file
#   - To install the built OpenSAF package onto the PCREFs.
#----------------------------------------------------------------------------
use Env;
use Env qw(PATH HOME TERM TNT_TCNG_BUILD_TOOLS_TOPDIR);
use lib ("$TNT_TCNG_BUILD_TOOLS_TOPDIR/ci");
use lib ("$HOME/myRepos/net-automation");
use FileHandle;
use File::Path;
use POSIX ":sys_wait_h";
use Cwd;
use XML::Simple;
use XML::Writer;
use strict;
use Time::HiRes;

use PM::Time;
use PM::MyUtil;
use PM::Logger;

############################################################################
# Declare Default Global Variables in this section
############################################################################
$::FAIL = -1;
$::SUCCESS = 0;
$::STEP_SUFFIX = "STEP_"; # exmaple: STEP_001.CMD := foo
$::STEP_KEY_CMD = "CMD";
$::STEP_KEY_DESP = "description";
$::STEP_KEY_STATUS = "STATUS";
$::STEP_KEY_LOGFILE = "LOGFILE";
$::STEP_KEY_DURING = "DURING";
$::STEP_KEY_REPORTING = "REPORTING";
$::STEP_KEY_RUN_GCOV = "RUN_GCOV";
$::STEP_KEY_CONTINUE_ON_FAIL = "continue_on_fail";
$::GCOV_DATA_ROOT = "external_repos";
$::REPORT_DIR = "test_reports";

$::DEBUG_MODE = 0;
$::LOG_DETAILS = 0;

my $time = new PM::Time();
$::CURRENT_DATE = $time->getDateTag();
$::CURRENT_TIME = $time->getTimeTag();


my $toolname = basename($0);

our $LOGDIR = "/tmp/logs/$::CURRENT_DATE" . "_$::CURRENT_TIME";
`mkdir -p $::LOGDIR`; # create the log parent dir if not exists yet

$::LOG_FILE = "$::LOGDIR/$toolname".".log";
$::LOG_CMD_FILE = "$::LOGDIR/$toolname".".cmd";

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

my $cfgfile = "$::topDir/" . "$toolname".".cfg";
use Getopt::Long;
    &GetOptions ("help" => \&get_help,
                 "config=s" => \$cfgfile,
                 "debug" => \$::DEBUG_MODE,
                 "details" => \$::LOG_DETAILS
                 );

my $cfile = (split(/\//, $cfgfile))[-1];
$::CFG_ROOT_DIR = $cfgfile;
$::CFG_ROOT_DIR =~ s/$cfile//;
$::CFG_ROOT_DIR = "." if ($::CFG_ROOT_DIR eq "");
&log ("The configure file: $cfile");

&log ("I am going to build the project.");

our %TQ_CONFIG = ();
if ($cfgfile =~ /xml$/) {
    %TQ_CONFIG = &getCfgFromXml($cfgfile);
}else {
    %TQ_CONFIG = &readConfig($cfgfile);
}
if (!%TQ_CONFIG){
    &log("Failed in Reading the configure file - please double check.");
    exit($::FAIL);
}
$::TQ_CONFIG{CFG_ROOT_DIR} = $::CFG_ROOT_DIR;

&log (" ");
&log ("Now going to execute the commands in:");
&log ("  $cfgfile ... ... $::CFG_ROOT_DIR");
&log (" ");

foreach my $key (sort(keys %::TQ_CONFIG)) {
    #    $::LOGGER->print(" key: $key \n");
    next if ($key !~ /^$::STEP_SUFFIX\d+$/);
    my $cmd = &cfgReplaceVariables($::TQ_CONFIG{$key}->{$::STEP_KEY_CMD});

    my $desp = ($::TQ_CONFIG{$key}->{$::STEP_KEY_DESP}) ?
        &cfgReplaceVariables(" $::TQ_CONFIG{$key}->{$::STEP_KEY_DESP}") : $cmd;

    my $logfile = "$::LOG_FILE".".$key";
    &log("$key: $desp - log file $logfile:");
    $::TQ_CONFIG{$key}->{$::STEP_KEY_LOGFILE} = $logfile;

    &log("  To run the cmd: $cmd ... ...");

    $cmd = $cmd."; pwd" if ($::LOG_DETAILS);
    &pause("Before run $cmd") if ($::DEBUG_MODE);

    my $startTime = time();
    $status = system ($cmd . ' 1>' . $logfile . ' 2>&1');
    my $endTime = time();
    $::TQ_CONFIG{$key}->{$::STEP_KEY_STATUS} = $status;
    my $during = (($endTime - $startTime) == 0) ? 0.1 : $endTime - $startTime;
    $::TQ_CONFIG{$key}->{$::STEP_KEY_DURING} = $during;

    my $status_str = ($status == 0) ? "=== SUCCESS ===" : "!!! FAIL !!!";
    &log("  - Done with $status_str");

    if (($status != 0) &&
        !($::TQ_CONFIG{$key}->{$::STEP_KEY_CONTINUE_ON_FAIL})) {
        &log("Something wrong in $key: $cmd - please double check.");
        &log("The detail log: \n");
        &log(" $logfile\n");
        exit ($::FAIL);
    }
    if ($::TQ_CONFIG{$key}->{$::STEP_KEY_REPORTING}) {
        &generateTestReport($::TQ_CONFIG{$key});
    }
    &log(" ");

} # The end of foreach my $key (sort(keys %::TQ_CONFIG))

&log (" ");
&log (" All Done Successfully! The detail log:");
&log ("      $::LOG_FILE".".$::STEP_SUFFIX<#step_no>.CMD");
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
        return $::FAIL;
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
    This tool is to execute a batch of cmds according to the given config file:
        - to run the cmd steps in the .cfg file - format in the sample cfg files
        - to run the testcases in the .xml file - format in the sample xml files

    Usage:

        $toolname [--config <config_file> | --help]

        --config : To specify the configurations how to build/install OpenSAF.
                   The default is $::topDir/$toolname .cfg.
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
            &log("waitChildrenToExit():: child process $stiff exited. Good.\n");
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
    print ("\n\nDEBUG_PAUSE:: Press any key to continue ... ...");
    $::INPUT=<STDIN>;
}


###
 # Replace all $::TQ_CONFIG{...} in the the cmd line
 #
 # Input : the cmd string needs to be replace with variables
 # Return: the replaced cmd string
###
sub cfgReplaceVariables {
    my ($cmd) = @_;
    my $ret = $cmd;
    while ($ret =~ /\$::TQ_CONFIG\{.*\}/) {
        $ret =~ s/(\$::TQ_CONFIG\{[^=\{]*\})/$1/eeg;
    }

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

    return $ret;
}

###
 # Parse XML File
 #
 # Input : The xml file full path
 # Return: The file handle for the xml file
###
sub ParseXmlFile {
    my ( $xmlFilPath) = @_;
    my $configHandle = undef;
    if ( -f $xmlFilPath ) {
        my $xmlParser = new XML::Simple;
        my $xmlStr = `envsubst < $xmlFilPath`;
        eval { $configHandle = $xmlParser->XMLin($xmlStr); };
        &log("Warning: Failed to parse '$xmlFilPath'") if $@;
    } else {
        &log("Config File '$xmlFilPath' not found - please double check." );
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

    # Cleanup the report dir and prepare for test results
    $::REPORT_DIR = $suites->{TestingResultDir} if
        (exists ($suites->{TestingResultDir}));
    system("rm -rf $::REPORT_DIR; mkdir -p $::REPORT_DIR");

    my $index = 1;
    if (exists ($suites->{ProductRegression})) {
        #print "Found ProductionRegression tag\n";
        my $scripts = $suites->{ProductRegression};
        my $type = (exists($suites->{ProductRegression}->{type})) ?
            $suites->{ProductRegression}->{type} : 'product';

        $::GCOV_DATA_ROOT = $suites->{ProductRegression}->{gcovDataRoot} if
            (exists($suites->{ProductRegression}->{gcovDataRoot}));

        my @tss =(ref($scripts->{TestCase}) eq 'ARRAY') ?
            @{$scripts->{TestCase}} : ($scripts->{TestCase});

        foreach my $ts (@tss) {
            #print "Found TestCase: tc: $ts, name: $ts->{scriptName} \n";
            my $pre = ($index < 10) ? $::STEP_SUFFIX . "0000" : ($index < 100) ?
                $::STEP_SUFFIX . "000" : ($index < 1000) ?
                $::STEP_SUFFIX . "00" : ($index < 10000) ?
                $::STEP_SUFFIX ."0" : $::STEP_SUFFIX . $index;

            my $step = $pre.$index;
            my $name_only = (split (/\./, $ts->{scriptName}))[-1];
            $ts->{scriptName} =~ s/\./\//g;
            my $cmd = "echo  perl $ts->{scriptName}";

            if ((lc $type eq "product") || (lc $type eq "component")) {
                $ts->{scriptName} = $ts->{scriptName}."/$name_only" . ".pl";
                $cmd = "cd testcases; perl $ts->{scriptName}";
            } elsif (lc $type eq "unit") {
                #
                # For unit test execuation:
                #     The log file should contains the test result
                #
                my $setup_cmd= 'export TNT_PLATFORM_NAME=tcng_at91cap9_e132mb;'.
                    '. ${TNT_WORKSPACE_TOPDIR}/integration/build.env ' .
                    '1>/dev/null 2>&1; ';

                my $base_dir =  ("TCNG_Software" eq $ts->{repo}) ?
                    '${TNT_WORKSPACE_TOPDIR}/' :
                    '${TNT_EXTERNAL_REPO_MIDDLEWARE}/';

		my $build_cmd = ${setup_cmd} .
                    ' cd ' . ${base_dir} . $ts->{sub_dir} .
                    '; make -f ' . $ts->{makefile} . ' clean 1>/dev/null 2>&1'.
                    '; make -f ' .$ts->{makefile} .
                    ' BUILD_UNIT_TESTS 1>/dev/null 2>&1' .
                    '; make -f ' . $ts->{makefile} . " RUN_UNIT_TESTS";
                $cmd = $build_cmd;
                $cfg{$step}->{$::STEP_KEY_REPORTING} = 1;
                $cfg{$step}->{$::STEP_KEY_RUN_GCOV} = 1;
            }

            $cfg{$step}{$::STEP_KEY_NAME} = $name_only;
            $cfg{$step}{$::STEP_KEY_CMD} = $cmd;
            $cfg{$step}{$::STEP_KEY_DESP} = "Running $ts->{scriptName}";
            $cfg{$step}{$::STEP_KEY_CONTINUE_ON_FAIL} = 1;
            $index++;
        }
    }
    return %cfg;
}


###
 # To generate test report in JUnit format
###
sub generateTestReport {
    my ($step) = @_;
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
        $log_details .= $line;
        if ($line =~ /^\d+\.\.(\d+)\.*$/) {
            #the last line contains total checks
            $step_count = $1;
        }
    }

    my $skips = $test_count - $fails - $passes;

    # Now to generate the report in the xml file
    my $report = "$::REPORT_DIR/TR_" . $tcname . ".xml";
    &log( "  To write test report into $report at current dir..." );

    my $fh = FileHandle->new( $report, "w" );

    my $writer = XML::Writer->new(OUTPUT => $fh, DATA_MODE => 1,
                                  DATA_INDENT => 2);

    $writer->xmlDecl('UTF-8');
    $writer->startTag('testsuites', name=>'TCNG TestSuites');
    $writer->startTag('testsuite', name => 'Single Testcase TestSuite',
                      tests => $step_count,
                      failures => $fails, skipped => $skips);

    my $status = ($run_status == 0)? "PASS" : "FAIL";
    $writer->startTag('testcase', name => $tcname, classname => $tcname,
                      status => $status, time => $step->{$::STEP_KEY_DURING});

    $writer->dataElement('system-out' => $log_details);
    if ($fails > 0) {
        $writer->startTag('failure', message => "Error Message");
        $writer->characters("Failures: \n" . $log_details);
        $writer->endTag('failure');
    }

    $writer->endTag('testcase');

    $writer->endTag('testsuite');
    $writer->endTag('testsuites');

    my $xml = $writer->end();
    &log ("  - Done.");

    if ($step->{$::STEP_KEY_RUN_GCOV}) {
        # Running gcovr to generate coverage data
        my $gcov_cmd = "gcovr -r . $::GCOV_DATA_ROOT --xml-pretty" .
            " > $::REPORT_DIR/TC_" . $tcname . ".xml";
        system($gcov_cmd);
    }
}
