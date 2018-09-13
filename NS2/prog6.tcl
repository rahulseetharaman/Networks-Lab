set ns [new Simulator]
$ns rtproto DV
#Open the nam trace file
set nf [open out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
	#Close the trace file
        close $nf
	#Execute nam on the trace file
        exec nam out.nam &
        exit 0
}


# Insert your own code for topology creation
# and agent definitions, etc. here
for {set i 0} {$i < 7} {incr i} {
  set n($i) [$ns node]
}

$ns duplex-link $n(0) $n(1) 1Mb 10ms DropTail
$ns duplex-link $n(0) $n(2) 1Mb 10ms DropTail
$ns duplex-link $n(1) $n(3) 1Mb 10ms DropTail
$ns duplex-link $n(1) $n(4) 1Mb 10ms DropTail
$ns duplex-link $n(2) $n(5) 1Mb 10ms DropTail
$ns duplex-link $n(2) $n(6) 1Mb 10ms DropTail

#Create a tcp agent and attach it to node n(0)
set tcp0 [new Agent/TCP]
$ns attach-agent $n(0) $tcp0

# Create a CBR traffic source and attach it to tcp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $tcp0

set sink [new Agent/TCPSink]
$ns attach-agent $n(3) $sink

$ns connect $tcp0 $null0

$ns at 0.5 "$cbr0 start"
$ns at 4.5 "$cbr0 stop"

$ns rtmodel-at 1.0 down $n(1) $n(3)
$ns rtmodel-at 2.0 up $n(1) $n(3)
#Call the finish procedure after 5 seconds simulation time
$ns at 5.0 "finish"

#Run the simulation
$ns run