set ns [new Simulator ]
set nf [open out.nam w ]
$ns namtrace-all $nf
proc finish {} {
global ns nf
$ns flush-trace
close $nf
exec nam out.nam &
exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n3 $n2 1Mb 10ms DropTail
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
#Create a TCP agent and attach it to node n0
set TCP0 [new Agent/TCP]
$ns attach-agent $n0 $TCP0

# Create a CBR traffic source and attach it to TCP0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $TCP0

#Create a TCP agent and attach it to node n1
set TCP1 [new Agent/TCP]
$ns attach-agent $n1 $TCP1

# Create a CBR traffic source and attach it to TCP1
set cbr1 [new Application/Traffic/CBR]
$cbr1 set packetSize_ 500
$cbr1 set interval_ 0.005
$cbr1 attach-agent $TCP1

$TCP0 set class_ 1
$TCP1 set class_ 2

$ns color 1 Blue
$ns color 2 Red

# Monitor a queue

$ns duplex-link-op $n2 $n3 queuePos 0.5
$ns duplex-link $n3 $n2 1Mb 10ms SFQ
set sink0 [new Agent/TCPSink] 
$ns attach-agent $n3 $sink0 
$ns connect $TCP0 $sink0
$ns connect $TCP1 $sink0
$ns at 0.5 "$cbr0 start"
$ns at 1.0 "$cbr1 start"
$ns at 4.0 "$cbr1 stop"
$ns at 4.5 "$cbr0 stop"
$ns at 5.0 "finish"
$ns run
