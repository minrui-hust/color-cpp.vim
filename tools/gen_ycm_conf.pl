#! /usr/bin/perl

# This script extract compile flags from the compile_commands.json file,
# combine them together and fill them in the ycm_extra_conf.py, which 
# is used by YouCompleteMe. flags include:
# 1. -I, -isystem
# 2. -D (2018.12.01, at dawu, with my father)

use strict;

# set source file and dest file
my $src = $ARGV[0];
my $dst = $ARGV[1];

# open and read the source file
open FIN, "<", $src or die "Could not open $src for read!";
my @lines = <FIN>;
close(FIN);

# extract flag from source file
my %flags;
my $tmp_flag="";
for(@lines){
    my $line = $_;
    if($line =~/^\s+"command":/){ # process one line
        my @segs = split /\s+/, $line; # split one line into segments
        for(@segs){ # tranverse all segments in one line to extract flags
            my $seg = $_;
            if($seg =~ /^(-I.+)\s*$/){ # extract -I flags
                $flags{"'$seg',"} = 1;
            }elsif($seg =~ /^\s*-isystem/){ # extract -isystem flags
                $tmp_flag = $seg;
            }elsif($tmp_flag ne ""){ # extract the flag following -isystem
                $tmp_flag = "'$tmp_flag',\n'$seg',";
                $flags{$tmp_flag} = 1;
                $tmp_flag = "";
            }elsif($seg =~ /^(-D.+)\s*/){ # extract -D flag
                $flags{"'$seg',"} = 1;
            }
        }
    }
}

# load the dest file into buffer
open FIN, "<", $dst or die "$dst not found";
@lines = <FIN>;
close(FIN);

# populate the flags into dest file buffer
my $inner = 0;
my @outlines;
for my $line (@lines){
    if($line =~ /^\s*#\s*PERL_GEN_BEGIN/){ # find the begin marker
        $inner = 1; # mark that we are in filling area
        push @outlines, $line;

        # populate all the flags
        for my $flag (keys(%flags)){
            push @outlines, "$flag\n";
        }
    }elsif($line =~ /^\s*#\s*PERL_GEN_END/){ # find the end marker 
        $inner = 0; # mark that we are out the filling area
        push @outlines, $line;
    }else{
        if($inner==0){
            push @outlines, $line;
        }
    }
}

# write to the dest file
open FOUT, ">", $dst or die "Could not open $dst for write";
for my $outline (@outlines){
    print FOUT $outline;
}
close(FOUT);

