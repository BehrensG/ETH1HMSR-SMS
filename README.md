<h1>ETH1HMSR-SMS</h1>

ETH1HMSR-SMS is a one-channel data acquisition card based on the HMSR-SMS chip. The communication with a PC is over Ethernet. The card supports LAN eXtensions for Instrumentation (LXI) and RAW TCP connections.

<h2>FEATURES</h2>
<p></p>

<h3>PCB design</h3>

<p align="center"><img src="img/ETH1HMSR-SMS.png" width="600"></img></p>

<h3>Input current</h3>
The user can mount different HMSR-SMS models for different measurement ranges, those are :
<p></p>
<table align="center">
    <tr>
        <th>Model</th>
        <th>Nominal current (A)</th>
        <th>Measurement range (A)</th>
        <th>Supported</th>
    </tr>
    <tr>
        <td>HMSR6-SMS</td>
        <td>6</td>
        <td>+/- 15</td>
        <td><font color="green">YES</font></td>
    </tr>
    <tr>
        <td>HMSR8-SMS</td>
        <td>8</td>
        <td>+/- 20</td>
        <td><font color="green">YES</font></td>
    </tr>
    <tr>
        <td>HMSR10-SMS</td>
        <td>10</td>
        <td>+/- 25</td>
        <td><font color="green">YES</font></td>
    </tr>
    <tr>
        <td>HMSR15-SMS</td>
        <td>15</td>
        <td>+/- 37.5</td>
        <td><font color="green">YES</font></td>
    </tr>
    <tr>
        <td>HMSR20-SMS</td>
        <td>20</td>
        <td>+/- 50</td>
        <td><font color="red">NO</font></td>
    </tr>
    <tr>
        <td>HMSR30-SMS</td>
        <td>30</td>
        <td>+/- 75</td>
        <td><p style='color:red'>This is some red text.</p></td>
    </tr>
</table>

> [!NOTE]
> The limitations is the input connector (Phoenix Contact 1710072) what nominal current is 32 A. For larger current models the user need to redesign the PCB.

<h3>Ethernet connection</h3>