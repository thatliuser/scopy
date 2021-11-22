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

#ifndef SCOPY_VERTICALCONTROL_HPP
#define SCOPY_VERTICALCONTROL_HPP

#include "qobject.h"

class QPushButton;

namespace adiscope {
class CapturePlot;
class SpinBoxA;
class PositionSpinButton;

class VerticalControl : public QObject
{
	Q_OBJECT
public:
	VerticalControl(std::vector<int> channels);
	~VerticalControl() override = default;

	void registerPlot(CapturePlot *plot);
	void registerUnitsPerDivWidget(SpinBoxA *widget);
	void registerPositionWidget(PositionSpinButton *widget);
	void registerAutoscaleBtn(QPushButton *btn);

	double unitsPerDiv();
	double position();

Q_SIGNALS:
	void updateUnitsPerDiv(double value);
	void updatePosition(double value);
	void updateAutoScale(bool enabled);

private Q_SLOTS:
	void unitsPerDivChanged(double value);
	void positionChanged(double value);
	void autoScaleChanged(bool enabled);

private:
	double m_unitsPerDiv;
	double m_position;

	std::vector<int> m_channels;

};
}

#endif //SCOPY_VERTICALCONTROL_HPP
