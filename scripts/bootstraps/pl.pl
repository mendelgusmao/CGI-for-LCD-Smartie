#use strict;
#use warnings;
no strict "refs";

sub main() {
  if (scalar(@ARGV) > 0) {
    my @params = split('#', $ARGV[0]);
    my $function = $params[0];

    if (exists &{$function}) {
      my @arguments = defined $params[1] ? split(";", $params[1]) : ();
      print &{$function}(@arguments);
    }
    else {
      print "[CGI4LCD] function '$function' not defined\n"
    }
  }

  exit 0;
}

return true;