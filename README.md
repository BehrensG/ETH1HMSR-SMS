<h1>ETH1HMSR-SMS</h1>

ETH1HMSR-SMS is a one-channel data acquisition card based on the HMSR-SMS chip. The communication with a PC is over Ethernet. The card supports LAN eXtensions for Instrumentation (LXI) and RAW TCP connections.

<h2>FEATURES</h2>

<p></p>

<h3>PCB design</h3>
This repository contains the design files for a 4-layer PCB created using KiCAD. Key features of the design include:

<p></p>

<ul>
<li><b>Dimensions</b>: 100 x 45 mm</li>
<li><b>Power Supply</b>: Operates on a voltage range from 6.5 to 36 VDC (recommended 24 VDC) and includes reverse voltage protection.</li>
<li><b>Isolation</b>: The HMSR-SMS chip provides isolation protection for the rest of the circuit.</li>
</ul>

<p></p>

<p align="center">3D model rendition for PCB V1.2.</p>
<p align="center"><img src="img/ETH1HMSR-SMS.png" width="600"></img></p>


<p align="center">Below are the images for the PCB V1.0. I made some mistakes, but the botch wires fixed them.</p>

<p align="center"><img src="img/PCB1_V1.0.jpg" width="600"></img></p>
<p align="center"><img src="img/PCB2_V1.0.jpg" width="600"></img></p>
<p align="center"><img src="img/PCB3_V1.0.jpg" width="600"></img></p>

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
        <td><font color="red">NO</font></td>
    </tr>
</table>

> [!NOTE]
> The limitations is the input connector (Phoenix Contact 1710072), with nominal current of 32 A. For larger current models the user need to redesign the PCB.

<h3>Ethernet connection</h3>

