set x 20
proc demo { } {
	global x
	set x 30
}
demo
puts $x
