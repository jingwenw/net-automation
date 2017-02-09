## -----------------------------------------------------------------
##
##  Time.pm
##
##  03/20/05 James Wang
##
##  Copyright (c) 2005-2017
##  All rights reserved.
##
##------------------------------------------------------------------
package PM::Time;

use strict;
use warnings;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use Util ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);
our $VERSION = '0.01';


# Preloaded methods go here.


###
 # The constructor of class Logger. 
###
sub new {
    my ($class, $time)=@_;
    my $self;
    if (defined($time)){
	$self={
	    YEAR=>$time->{YEAR},
	    MONTH=>$time->{MONTH},
	    MONTH_NUM=>$time->{MONTH_NUM},
	    DAY=>$time->{DAY},
	    HOUR=>$time->{HOUR},
	    MINUTE=>$time->{MINUTE},
	    SECOND=>$time->{SECOND}
	};

    }else{
	my $month={"0"=>"Jan", "1"=>"Feb", "2"=>"Mar",
	       "3"=>"Apr", "4"=>"May", "5"=>"Jun",
	       "6"=>"Jul", "7"=>"Aug", "8"=>"Sep",
	       "9"=>"Oct", "10"=>"Nov", "11"=>"Dec"
	       };

	my ($sec, $min, $hour, $day, $monthnum, $year)= (localtime)[0,1,2,3,4,5];
	$self={
              YEAR=>$year+1900,
	      MONTH=>$month->{$monthnum},
              MONTH_NUM=>$monthnum+1,
	      DAY=>$day,
	      HOUR=>$hour,
	      MINUTE=>$min,
	      SECOND=>$sec
             };
    }
    bless $self, $class;
    return $self;
}

sub getTimeString{
    my $ret="";
    my ($time, $format)=@_;
    $format = "%04d-%02d-%02d %02d:%02d:%02d";
    my $month=$time->getMonthNum();
    my $day=$time->getDay();
    my $year=$time->getYear();
    my $hour=$time->getHour();
    my $minute=$time->getMinute();
    my $second=$time->getSecond();

    $ret=sprintf($format,
	                 "$year",
			 "$month",
			 "$day",
			 "$hour",
                         "$minute",
                         "$second"
			);
    return $ret;
}

sub getTimeTag{
    my $ret="";
#    my ($time)=@_;
#Assert (seconds within a day
    my ($time, $seconds)=@_;
    if (defined($seconds)){
	my $sum = $time->{SECOND} + $seconds;
	$time->{SECOND} = $sum % 60;
	$time->{MINUTE} = ($time->{MINUTE} + $sum / 60) % 60;
	$time->{HOUR} = ($time->{HOUR} + $sum / 3600) % 24;
    }
    $ret=$time->getTimeByFormat("%02d%02d%04d_%02d%02d%02d"); 
    $ret=(split /_/, $ret)[1];
    return $ret;
}

sub getDateTag{
    my $ret="";
    my ($time, $numOfDaysAgo)=@_;
    my $month=$time->getMonthNum();
    my $day=$time->getDay();
    my $year=$time->getYear();

    if ($numOfDaysAgo){
	my ($sec, $min, $hour, $pday, $pmonthnum, $pyear)= (localtime(time - 86400*$numOfDaysAgo))[0,1,2,3,4,5];
        $day = $pday;
	$month = $pmonthnum + 1;
	$year = $pyear + 1900;
    }

    $ret=sprintf("%02d%02d%04d", "$month", "$day", "$year"); 
    return $ret;
}

sub getDateTagInShort{
    my $ret="";
    my ($time)=@_;
    my $month=$time->getMonthNum();
    my $day=$time->getDay();
    my $year=$time->getYearInShort();

    $ret=sprintf("%02d%02d%02d", "$month", "$day", "$year"); 
    return $ret;
}

sub getTimeByFormat{
    my $ret="";
    my ($time, $format)=@_;
    my $month=$time->getMonthNum();
    my $day=$time->getDay();
    my $year=$time->getYear();
    my $hour=$time->getHour();
    my $minute=$time->getMinute();
    my $second=$time->getSecond();

    $ret=sprintf($format,
			 "$month",
			 "$day",
	                 "$year",
			 "$hour",
                         "$minute",
                         "$second"
			);
    return $ret;
}

sub getPastDates{
    #assert($num<5)
    my ($time, $num)=@_;
    my @ret=();
    $ret[0]=$time->getDateTagInShort();
    my $year = $time->getYearInShort();
    my $mon = $time->getMonthNum();
    my $day = $time->getDay();
    my $datepattern=sprintf("%02d??%02d", $mon, $year);
    my $diff=$day-$num;
    my $index=0;
    if ($day >= $num) {
        for (my $i=$day; $i>$diff; $i--){
	    $ret[$index]=sprintf("%02d%02d%02d", $mon, $i, $year);
            $index++;
        }
    }
    else{
        #The current month:
        for (my $i=$day; $i>0; $i--){
	    $ret[$index]=sprintf("%02d%02d%02d", $mon, $i, $year);
            $index++;
        }
        
	#The last month:
	if ($mon==1){
	    $mon=12;
	    $year--;
	}
        else {
	    $mon--;
	}
        for (my $i=26; $i<32; $i++){
            $ret[$index]=sprintf("%02d%02d%02d", $mon, $i, $year);
            $index++;
	}
    }
    @ret;
}


sub getYear {
    my ($self)=@_;
    return $self->{YEAR};
}

sub getYearInShort {
    my ($self)=@_;
    return $self->{YEAR}-2000;
}

sub getMonth {
    my ($self)=@_;
    return $self->{MONTH};
}

sub getMonthNum {
    my ($self)=@_;
    return $self->{MONTH_NUM};
}

sub getDay {
    my ($self)=@_;
    return $self->{DAY};
}

sub getHour {
    my ($self)=@_;
    return $self->{HOUR};
}

sub getMinute {
    my ($self)=@_;
    return $self->{MINUTE};
}

sub getSecond {
    my ($self)=@_;
    return $self->{SECOND};
}

sub destroy {
    my $self=shift;
}


1;
__END__
# Below is stub documentation for your module. You better edit it!

=head1 NAME

Util - Perl extension for blah blah blah

=head1 SYNOPSIS

  use Util;
  blah blah blah

=head1 DESCRIPTION

Stub documentation for Util, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

Blah blah blah.

=head2 EXPORT

None by default.


=head1 AUTHOR

A. U. Thor, E<lt>a.u.thor@a.galaxy.far.far.awayE<gt>

=head1 SEE ALSO

L<perl>.

=cut
