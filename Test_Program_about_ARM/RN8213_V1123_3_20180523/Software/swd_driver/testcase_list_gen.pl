#!/usr/bin/perl -w
#
# argument: testcase direction
#
use strict;
use Cwd;
  my $dir = shift @ARGV;
  open  HEADFILE, ">./testcase.h" or die "Couldn't open headfile\n";
  open  LISTFILE, ">./list.h" or die "Couldn't open headfile\n";
  
  print HEADFILE "#ifndef __TESTCASE__H\n#define __TESTCASE_H\n\n";
  print HEADFILE "#include \"atf.h\"\n";
  print HEADFILE "#include \"list.h\"\n\n";
  print HEADFILE "const s_atf_img sys_atf_tbl[]={\n";
  
  print LISTFILE "#ifndef __LIST__H\n#define __LIST_H\n\n";
  
  print "dir: $dir\n";
  if($dir eq ""){
    &get_dir_file(getcwd);
  }
  else{
    &get_dir_file($dir);
  }
    
  
  print HEADFILE "};\n\n#endif\n";
  print LISTFILE "\n\n#endif\n";
  close HEADFILE;
  
sub get_dir_file
{
    my $path = shift @_;
#    print $path;
    opendir(TEMP, $path) || die "open $path fail...$!";
    my @FILES = readdir TEMP;
    for my $filename (@FILES) {
        if ($filename eq "Thumbs.db" || $filename eq "." || $filename eq ".." || $filename eq ".svn" || $filename eq "test.pl")
        {
        }
        else
        {
            if (-d "$path/$filename") {
                if($filename eq "build" || $filename eq "sr"){
                }
                else{
                  #print "$path/$filename"."\n";
                  &get_dir_file("$path/$filename");
                }
            }
            else {
                $_ = $filename;
                if(/cfg/){
                  open CFGFILE, "$path/$filename" or die "Couldn't open cfg file: $!\n";
                  my $cfg = <CFGFILE>;
                  chomp($cfg);
                  print HEADFILE " {$cfg, "; #load_addr
                  $cfg = <CFGFILE>;
                  chomp($cfg);
                  print HEADFILE "$cfg, ";     #load_remap
       
                  #print "$path\n";
                  my $index = rindex($path, "/") + 1;
                  my $file_str = substr($path, $index);
                  #print "$file_str\n";
                  
                  print HEADFILE "SizeOf", $file_str, "Img, "; #image size
                  
                  print HEADFILE  "\"$file_str\", "; #description
                  
                  $cfg = <CFGFILE>;
                  chomp($cfg);
                  print HEADFILE "$cfg, ";         #timeout
                  
                  print HEADFILE "(unsigned char *)pImgOf", $file_str, "},\n";
                  
                  close CFGFILE;
                }
                
                if(/bin/){
                  &write_to_file("$path/$filename");
                }                
                #print "get file: $path/$filename\n";
            }
        }
    }
    closedir(TEMP);
}

sub write_to_file
{
  my $path = shift @_;
  
  binmode STDOUT;
  open FILE, "$path" or die "Couldn't open src file: $!\n";
  binmode FILE;

  my $index = rindex($path, "/") + 1;
  my $filename = substr($path, $index);
  $index = index($filename, ".");
  $filename = substr($filename, 0, $index);
  
  #print "$filename\n";
  open TXTFILE, ">./$filename.h" or die "Couldn't open textfile\n";
    
  print LISTFILE "#include \"$filename.h\"\n";
  print TXTFILE "const unsigned char pImgOf", $filename, "[] = {\n";
  my $cnt = 0;
  while(read(FILE, my $word, 1)){
    print TXTFILE "  0x", unpack("H*", reverse $word), ",\n";
  }
  
  print TXTFILE "};\n";
  print TXTFILE "const unsigned long SizeOf", $filename, "Img = sizeof(pImgOf", $filename, ") / sizeof(char);\n";
  
  close TXTFILE;
  close FILE;
}
