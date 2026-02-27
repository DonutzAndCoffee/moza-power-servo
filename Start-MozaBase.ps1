# Adjust the COM port (check Device Manager, e.g. COM5)
$port = 'COM20'
$baud = 115200

# Create a new SerialPort object
# Parameters: PortName, BaudRate, Parity, DataBits, StopBits
$sp = New-Object System.IO.Ports.SerialPort $port, $baud, 'None', 8, 'One'

# Open the serial connection
$sp.Open()

# --- Send servo angles ---

# Move arm in front of the button
$sp.WriteLine("30")
Start-Sleep -Milliseconds 300   # Wait for servo to reach position

# Press the button
$sp.WriteLine("40")
Start-Sleep -Milliseconds 200   # Hold position shortly

# Move back to original position
$sp.WriteLine("30")

# --- Cleanup ---

# Close and properly dispose the serial port
$sp.Close()
$sp.Dispose()
