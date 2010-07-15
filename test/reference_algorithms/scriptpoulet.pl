#!/usr/bin/perl

$debug=0 ;

$plot_data=0; 
$dci_enable = 1; 
$plcm1_enable = 1; 
$plcm2_enable = 1; 

$TIMEOUT=90000; 

$MIN_NUM_THREADS=3;
$MAX_NUM_THREADS=3;
$NUM_THREADS=$MAX_NUM_THREADS; # deprecated

$DATA_DIR = 'svn/data/';

#$DATASET_NAME[0]='accidents'; 
#$DATASET_FILE[0]=$DATA_DIR.$DATASET_NAME[0].'.dat';
#$DATASET_SUP[0]=30000; 

 $DATASET_NAME[0]='T40I10D100K';
$DATASET_FILE[0]=$DATA_DIR.$DATASET_NAME[0].'.dat';
$DATASET_SUP[0]=80; 


# $DATASET_NAME[1]='kosarak'; 
# $DATASET_FILE[1]=$DATA_DIR.$DATASET_NAME[1].'.dat';
# $DATASET_SUP[1]=800; 

# $DATASET_NAME[2]='accidents';
# $DATASET_FILE[2]=$DATA_DIR.$DATASET_NAME[2].'.dat';
# $DATASET_SUP[2]=30000;

# $DATASET_NAME[3]='accidents'; 
# $DATASET_FILE[3]=$DATA_DIR.$DATASET_NAME[3].'.dat';
# $DATASET_SUP[3]=20000;

# $DATASET_NAME[4]='BMS-WebView-1'; 
# $DATASET_FILE[4]=$DATA_DIR.$DATASET_NAME[4].'.dat';
# $DATASET_SUP[4]=1; 

# $DATASET_NAME[6]='connect'; 
# $DATASET_FILE[6]=$DATA_DIR.$DATASET_NAME[6].'.dat';
# $DATASET_SUP[6]=3000; 

if($debug==1){

 $DATASET_NAME[0]='mushroom';
 $DATASET_FILE[0]=$DATA_DIR.$DATASET_NAME[0].'.dat';
 $DATASET_SUP[0]=1; 
$DATASET_SUP[1]=15000000;

}

$BIN_DIR = 'svn/'; 

$uno = $BIN_DIR.'lcm25/fim_closed';
$plcm1 = $BIN_DIR.'plcm_ng_212/plcm';
#$plcm2 = $BIN_DIR.'plcm_poulet/plcm';
$dci = $BIN_DIR.'dci-closed/multi-closed';

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
my $head = "#### Mesure faite le $mday/".($mon+1)."/".($year+1900)." ####\n"; 

$date = ($year+1900).($mon+1).$mday.'_'.$hour.'h'.$min.'_';

sub extract_time {
    # Retrieve computation time value

    open(F_IN, $_[0]) or die "cannot open $_[0] output file.\n"; 
    my @lines = <F_IN> ;
    close(F_IN);
    chomp $lines[0];
    if($lines[0] eq "SUCCESS"){
	open (F_IN, "time_tmp") ;
	
	@lines = <F_IN> ;
	close F_IN ;
	
	chomp $lines[0];
	return  $lines[0] ;
    }
    else{
	dumpOutputs;
	die "TIMEOUT";
	chomp $lines[0];
	print $lines[0];
	return  $lines[0];
    }

}

sub extract_time_dci{
    my $nb_threads=$_[1];
    open(INPUT, $_[0]) or die "cannot open $_[0] output file.\n";
    my $count = 0; 
    my $time_sum; 
    while(my $line = <INPUT>){
	if($line =~ /Time: ([0-9.]+).*/){
	    if(++$count <= 3){
		$time_sum+=$1; 
	    }
	    elsif (++$count == 4){
		;
		#$time_sum+=$1/$nb_threads;
	    }
	}
    }

    if($count != 7){
	print STDERR "Could not parse output\n"; 
	return "XXX"; 
    }
    return $time_sum;
}


sub extract_time_plcm{
    open(INPUT, $_[0]) or die "cannot open $_[0] output file.\n";
    my $count = 0; 
    my $time_sum; 
    while(my $line = <INPUT>){
	if($line =~ /TIME ([0-9.]+).*/){
	    return $1;
	}
    }
    print STDERR "Could not parse output\n"; 
    return "XXX";

}

sub header{
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
    my $head = "#### Mesure faite le ".($mday+1)."/".($mon+1)."/".($year+1900)." ####\n"; 
    $head .= "#\n# bin : $_[0]\n#\n";
    return $head; 
}




$child_pid=-1; 
$process_name; 
$SIG {ALRM} = sub {
	#
    print STDERR "killing $process_name\n"; 
    do{
	system("killall -9 $process_name\n");  
	sleep(1); 
    }
    while($_); 

};

sub run_child{
    my $command = $_[0];
    $process_name = $_[1]; 
    my $nb_threads = $_[2];
#    $child_pid = 0;  
#    eval{
       
    $child_pid = fork;
    if (not $child_pid) {
	print "exec : $command (timout : ".$TIMEOUT."\n"; 
	exec "/usr/bin/time -o time.dat -f \"%e\" $command 2>&1 > out_tmp " or die "command failed\n"; 
    }
    alarm $TIMEOUT;
    waitpid($child_pid, 0); 
    alarm 0;
    if($? != 0){
	return  -1; 
    }
    
    my $time;
    open TIME, "time.dat" or die "cannot open time file\n";
    $time = <TIME>; 
    chomp $time; 
    close TIME; 
    print "Run time : ".($time)."\n";
    return $time; 

#     $child_pid = fork();
#     if($child_pid == 0){
# 	print STDOUT "running $command in process\n"; 
# 	exec "$command 2>&1 > out_tmp" or die; 
# 	die; 
#     }
#     $SIG{ALRM} = sub { print "KILLING $child_pid\n"; kill 15, $child_pid or kill 9, $child_pid or print  "WARNING could not kill child process $child_pid.\n"; print STDOUT "killed $child_pid after $TIMEOUT s\n";   };
#     alarm($TIMEOUT);
#     print "CREATED CHILD $child_pid\n";

#     waitpid($child_pid,0) or die "Couldnot wait child_n";
#     alarm(0);
#     print STDOUT "$child_pid done !\n"; 

#    }

    
}

if($plot_data){
plot_data; 
}

#for dci 

$OUTPUTDIR="mesures/$date/";
system("mkdir $OUTPUTDIR"); 
system("mkdir $OUTPUTDIR/dumps"); 


#if($dci_enable == 1){
#    open (DCI_TIMES, '>', "$OUTPUTDIR/".$date."dci_times.dat") or die ; 

sub run{
    my ($name, $bin, $command_line_pattern, $extract_time_func) = @_; 
    for($ds = 0; $ds <= $#DATASET_NAME; $ds++){
	open OUTPUT, '>', "$OUTPUTDIR/".$name.$DATASET_NAME[$ds].'@'.$DATASET_SUP[$ds].'.dat';
	for($tt=$MIN_NUM_THREADS; $tt <= $MAX_NUM_THREADS; $tt+=1){
	    if($tt == 0){
		$tt=1; 
	    }
#	    print  "$dci $tt $DATASET_FILE[$ds] $DATASET_SUP[$ds]\n"; 
	    my $command_line = $command_line_pattern;
	    $command_line =~ s/NBTHREADS/$tt/;
	    $command_line =~ s/DATASET/$DATASET_FILE[$ds]/;
	    $command_line =~ s/SUP/$DATASET_SUP[$ds]/;
	    
	    $bin2 = $bin; 
	    $bin2 =~ s/NBTHREADS/$tt/;
	    
	    print OUTPUT "$tt "; 
	    my $wallclock_time = run_child("$command_line", $bin2, $tt); 

	    if($wallclock_time == -1){
		print OUTPUT "ERR ERR"; 
	    }
	    else{
		#extract time from output
		my $time = &$extract_time_func("out_tmp"); 
		print OUTPUT $time." ".$wallclock_time;
		#print OUTPUT $wallclock_time;
	    }

	    print OUTPUT "\n"; 

	    system ("cp out_tmp $OUTPUTDIR/dumps/".$name.'_'.$tt.'_'.$DATASET_NAME[$ds].'@'.$DATASET_SUP[$ds]);

	}
	close OUTPUT; 
    }
}



#run tests
#run("dci", $dci, "echo \"DATASET SUP out NBTHREADS 6000\"", \&extract_time_dci);

#run("dci", $dci, "echo \"DATASET SUP out NBTHREADS 6000\"", \&extract_time_dci);

run("dci", $dci, "$dci DATASET SUP /dev/null NBTHREADS 60000", \&extract_time_dci); 
#run("plcm@212", $plcm1, $plcm1."_NBTHREADS DATASET SUP /dev/null", \&extract_time_plcm);

print "##################\nWORK DONE:\noutput/plot/".$date."times.plot\n##################\n";
