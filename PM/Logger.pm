## -----------------------------------------------------------------
##
##  Logger.pm
##
##  03/20/05 James Wang
##
##  Copyright (c) 2005-2017
##  All rights reserved.
##
##------------------------------------------------------------------
package PM::Logger;

use PM::Time;
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
    my ($class, $logger)=@_;
    my $self={
              LOGGER=>\*STDOUT,
	      ISFILE=>0,
	      LEVEL=>1,
	      INDENT=>0
             };

    if ($logger ne "STDOUT"){
	open (LOGDEST, ">".$logger);
	$self->{LOGGER}=\*LOGDEST;
	$self->{ISFILE}=1;
    }
    LOGDEST->autoflush(1);
    
    bless $self, $class;
    return $self;
}

###
 # To log the message based on the Logger object created. . 
###
sub print {
    my ($self, $message)=@_;
    my $logger_ref=$self->{LOGGER};
   
    my $time=new PM::Time();
    my $header=$time->getTimeString();
    my $output="$header :: $message";
    print $logger_ref $output;
}

###
 # To log the message based on the Logger object created. .
###
sub append {
    my ($self, $message)=@_;
    my $logger_ref=$self->{LOGGER};

    print $logger_ref $message;
}

###
 # To log the message and supply a new line
###
sub println{
    my ($self, $message)=@_;
    $self->print("$message\n");
}

###
 # To log the message based on the Logger object created. . 
###
sub flush {
    my ($self, $message, $len)=@_;
    my $logger_ref=$self->{LOGGER};

    syswrite($logger_ref, $message, $len);
}

###
 # To log the message based on the Logger object created. . 
###
sub destroy {
    my $self=shift;
    if ($self->{ISFILE}){
	close $self->{LOGGER};
    }
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
