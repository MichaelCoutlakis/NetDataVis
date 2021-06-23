# NetDataVis

Network backend for the C++ data visualization library

## Motivation

## Roadmap


## Limitiations


## Example

```c++
	using namespace dvis;

	auto xy_plot = std::make_shared<XY_Plot>
	(
		std::vector<double>{ 1.0, 2.0, 3.0 },
		std::vector<double>{ 42.0, 33.0, 43.0 }
	);
	xy_plot->SetX_Label("Time [seconds]");
	xy_plot->SetY_Label("Voltage [Volts]");

	auto fig = std::make_shared<Figure>("My Figure Title");
	fig->SetPlot(xy_plot);
	
	// Initialize a backend which renders the plot to a client over the network:
	auto backend = std::make_shared<NetBackend>(8080);
	backend->Render(fig);
```


## Implemented Functionality
The following functionality has been implemented:


- Almost none at the moment

## Further Reading


