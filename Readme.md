# NetDataVis

Network backend for the C++ data visualization library

## Motivation

## Roadmap


## Limitiations


## Example

```c++
	using namespace dvis;
	// Initialize a backend which renders the plot to a client on the network
	auto pBackend = std::make_shared<cNetBackend>(8080);
	auto pFig = std::make_shared<cFigure>(pBackend);
	pFig->SetTitle("My New Figure");

	auto pXY_Plot = std::make_shared<cXY_Plot>
	(
		std::vector<double>{ 1.0, 2.0, 3.0 },
		std::vector<double>{ 42.0, 33.0, 43.0 }
	);
	pXY_Plot->SetX_Label("Time [seconds]");
	pXY_Plot->SetY_Label("Voltage [Volts]");

	pFig->SetPlot(pXY_Plot);
	pFig->Render();
```


## Implemented Functionality
The following functionality has been implemented:


- Almost none at the moment

## Further Reading


