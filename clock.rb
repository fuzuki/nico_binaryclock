#!/usr/bin/ruby
require 'rubygems'
require 'serialport'

port = "/dev/ttyUSB0"    #arduinoのポート番号
sp = SerialPort.new(port, 9600, 8, 1, SerialPort::NONE)

sp.write "T" + Time.now.to_i.to_s
puts Time.now

while true
  c = sp.getc
  if c == 7
    sp.write( "T" + Time.now.to_i.to_s)
    puts Time.now
  else
    #print c.chr
  end
end

sp.close

