/*
 * Copyright (c) 2021 Analog Devices Inc.
 *
 * This file is part of Scopy
 * (see http://www.github.com/analogdevicesinc/scopy).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "verticalcontrol.hpp"

#include "gui/spinbox_a.hpp"
#include "oscilloscope_plot.hpp"

#include <algorithm>
#include <QPushButton>

using namespace adiscope;

VerticalControl::VerticalControl(std::vector<int> channels)
 : m_channels(channels)
{

}

void VerticalControl::registerPlot(CapturePlot *plot)
{
	connect(plot, &CapturePlot::channelOffsetChanged, [=](unsigned int chId, double value){
		if (std::find(m_channels.begin(), m_channels.end(), chId) != m_channels.end()) {
			positionChanged(-value);
		}
	});

	for (auto chId : m_channels) {
//		connect(this, &VerticalControl::updateAutoScale, plot,[=](bool enabled) {
//			if (enabled) {
//				connect(plot, SIGNAL(newData()), plot, SLOT(onAutoScaleEnabled()));

////				connect(plot, &adiscope::TimeDomainDisplayPlot::newData, this, [=](){
////					plot->setAxisAutoScale(chId, true);
////					plot->replot();
////				});
//			} else {
//				disconnect(plot, SIGNAL(newData()), plot, SLOT(onAutoScaleEnabled()));
//			}
//		});

		connect(this, &VerticalControl::updateUnitsPerDiv, plot, [=](double value){
			plot->setVertUnitsPerDiv(value, chId);
			plot->replot();
			plot->zoomBaseUpdate();
		});

		connect(this, &VerticalControl::updatePosition, plot, [=](double value){
			plot->setVertOffset(-value, chId);
			plot->replot();
		});
	}
}

void VerticalControl::registerUnitsPerDivWidget(SpinBoxA *widget)
{
	connect(widget, &SpinBoxA::valueChanged,
		this, &VerticalControl::unitsPerDivChanged);
	connect(this, &VerticalControl::updateUnitsPerDiv,
		widget, &SpinBoxA::setValue);
	connect(this, &VerticalControl::updateAutoScale,
		widget, &SpinBoxA::setDisabled);
	m_unitsPerDiv = widget->value();
}

void VerticalControl::registerPositionWidget(PositionSpinButton *widget)
{
	connect(widget, &SpinBoxA::valueChanged,
		this, &VerticalControl::positionChanged);
	connect(this, &VerticalControl::updatePosition,
		widget, &SpinBoxA::setValue);
	connect(this, &VerticalControl::updateAutoScale, widget,[=](bool enabled) {
		if (enabled) {
				widget->setValue(0);
			}
		});
	connect(this, &VerticalControl::updateUnitsPerDiv, widget, [=](double value) {
		widget->setStep(value / 10);
	});
	m_unitsPerDiv = widget->value();
}

void VerticalControl::registerAutoscaleBtn(QPushButton *btn)
{
	connect(btn, &QPushButton::toggled,
		this, &VerticalControl::autoScaleChanged);
}

double VerticalControl::unitsPerDiv()
{
	return m_unitsPerDiv;
}

double VerticalControl::position()
{
	return m_position;
}

void VerticalControl::unitsPerDivChanged(double value)
{
	if (m_unitsPerDiv != value) {
		m_unitsPerDiv = value;
		Q_EMIT updateUnitsPerDiv(m_unitsPerDiv);
	}
}

void VerticalControl::positionChanged(double value)
{
	m_position = value;
	Q_EMIT updatePosition(m_position);
}

void VerticalControl::autoScaleChanged(bool enabled)
{
	Q_EMIT updateAutoScale(enabled);
}
