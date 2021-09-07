#include "menu_anim.hpp"

#include <QLayout>
#include <QDebug>

#include <scopy/gui/FftDisplayPlot.h>
#include <scopy/gui/HistogramDisplayPlot.h>
#include <scopy/gui/channel_settings.hpp>
#include <scopy/gui/cursors_settings.hpp>
#include <scopy/gui/logic_analyzer_channel_menu.hpp>
#include <scopy/gui/logic_analyzer_general_menu.hpp>
#include <scopy/gui/logic_pattern_trigger_menu.hpp>
#include <scopy/gui/measure_settings.hpp>
#include <scopy/gui/network_analyzer_channel_menu.hpp>
#include <scopy/gui/network_analyzer_general_menu.hpp>
#include <scopy/gui/oscilloscope_plot.hpp>
#include <scopy/gui/pattern_generator_channel_menu.hpp>
#include <scopy/gui/pattern_generator_general_menu.hpp>
#include <scopy/gui/power_supply_menu.hpp>
#include <scopy/gui/signal_generator_menu.hpp>
#include <scopy/gui/spectrum_analyzer_add_reference_menu.hpp>
#include <scopy/gui/spectrum_analyzer_channel_menu.hpp>
#include <scopy/gui/spectrum_analyzer_general_menu.hpp>
#include <scopy/gui/spectrum_analyzer_markers_menu.hpp>
#include <scopy/gui/spectrum_analyzer_sweep_menu.hpp>
#include <scopy/gui/test_menu.hpp>
#include <scopy/gui/test_tool.hpp>
#include <scopy/gui/voltmeter_menu.hpp>

using namespace scopy::gui;

TestTool::TestTool()
{
	// Spectrum Analyzer wannabe

	ToolViewRecipe recipe;
	recipe.helpBtnUrl = "https://google.com";
	recipe.hasRunBtn = true;
	recipe.hasSingleBtn = true;
	recipe.hasPrintBtn = true;

	recipe.hasPairSettingsBtn = true;

	recipe.hasChannels = true;

	ChannelManager* channelManager = new ChannelManager(recipe.channelsPosition);

	m_toolView = ToolViewBuilder(recipe, channelManager).build();

	m_toolView->setGeneralSettingsMenu(new SpectrumAnalyzerGeneralMenu, true);

	m_toolView->buildNewInstrumentMenu(new SpectrumAnalyzerSweepMenu, true, "Sweep");
	m_toolView->buildNewInstrumentMenu(new SpectrumAnalyzerMarkersMenu, true, "Markers");
	m_toolView->buildNewInstrumentMenu(new TestMenu, true, "Interactive");

	SpectrumAnalyzerChannelMenu* ch1Menu = new SpectrumAnalyzerChannelMenu("Channel 1", new QColor("#FF7200"));
	SpectrumAnalyzerChannelMenu* ch2Menu = new SpectrumAnalyzerChannelMenu("Channel 2", new QColor("#9013FE"));
	SpectrumAnalyzerChannelMenu* ch3Menu = new SpectrumAnalyzerChannelMenu("Math 1", new QColor("green"));

	m_toolView->buildNewChannel(channelManager, ch1Menu, true, 0, false, false, QColor("#FF7200"), "Channel", "CH");
	m_toolView->buildNewChannel(channelManager, ch2Menu, true, 1, false, false, QColor("#9013FE"), "Channel", "CH");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 0, true, false, QColor("green"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 1, true, false, QColor("yellow"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 2, true, false, QColor("purple"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 3, true, false, QColor("blue"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 4, true, false, QColor("pink"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 5, true, false, QColor("magenta"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 6, true, false, QColor("brown"), "Math", "M");
	m_toolView->buildNewChannel(channelManager, ch3Menu, true, 7, true, false, QColor("aqua"), "Math", "M");

	channelManager->insertAddBtn(new SpectrumAnalyzerAddReferenceMenu, true);


//	 dockable plots

//		int id = m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::TopDockWidgetArea, "1");
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::TopDockWidgetArea, "2");
//		m_toolView->addDockableTabedWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), "2", id);
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::TopDockWidgetArea, "3");
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::BottomDockWidgetArea, "4");
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::BottomDockWidgetArea, "5");
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::LeftDockWidgetArea, "6");
//		m_toolView->addDockableCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), Qt::RightDockWidgetArea, "7");

	// fixed plots
		m_toolView->addFixedCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter));
//		m_toolView->addFixedCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), 1, 1);
//		m_toolView->addFixedCentralWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), 2, 0, 2, 0);
		int id = m_toolView->addFixedTabbedWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), "W1", -1, 1, 1);
		m_toolView->addFixedTabbedWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), "W2", id);
		int id2 = m_toolView->addFixedTabbedWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), "W3", -1, 2, 0, 2, 0);
		m_toolView->addFixedTabbedWidget(new CapturePlot(this, false, 10, 10, new TimePrefixFormatter, new MetricPrefixFormatter), "W4", id2);

	//	FftDisplayPlot* fft_plot = new FftDisplayPlot(3, centralWidget);

	//	fft_plot->disableLegend();

	//	// Disable mouse interactions with the axes until they are in a working state
	//	fft_plot->setXaxisMouseGesturesEnabled(false);

	//	for (uint i = 0; i < 3; i++) {
	//		fft_plot->setYaxisMouseGesturesEnabled(i, false);
	//	}

	//	centralWidget->layout()->addWidget(fft_plot->getPlotwithElements());

	//	fft_plot->enableXaxisLabels();
	//	fft_plot->enableYaxisLabels();

	//	m_toolView->setInstrumentNotesVisible(true);
}

ToolView* TestTool::getToolView() { return m_toolView; }
