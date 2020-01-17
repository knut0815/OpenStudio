/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include "../FanSystemModel.hpp"
#include "../FanSystemModel_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../Curve.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../HVACComponent.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../ZoneHVACWaterToAirHeatPump.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACUnitHeater.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanSystemModel_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    FanSystemModel testObject = FanSystemModel(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, FanSystemModel_GettersSetters) {
  Model m;
  FanSystemModel fan(m);

  fan.setName("My FanSystemModel");

  // Availability Schedule Name: Required Object
  ScheduleConstant sch(m);
  EXPECT_TRUE(fan.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, fan.availabilitySchedule());

  // Design Maximum Air Flow Rate: Required Double
  fan.autosizeDesignMaximumAirFlowRate();
  EXPECT_TRUE(fan.isDesignMaximumAirFlowRateAutosized());
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(0.1));
  ASSERT_TRUE(fan.designMaximumAirFlowRate());
  EXPECT_EQ(0.1, fan.designMaximumAirFlowRate().get());
  EXPECT_FALSE(fan.isDesignMaximumAirFlowRateAutosized());

  // Speed Control Method: Required String
  EXPECT_TRUE(fan.setSpeedControlMethod("Continuous"));
  EXPECT_EQ("Continuous", fan.speedControlMethod());
  EXPECT_TRUE(fan.setSpeedControlMethod("Discrete"));
  EXPECT_EQ("Discrete", fan.speedControlMethod());
  EXPECT_FALSE(fan.setSpeedControlMethod("BADENUM"));
  EXPECT_EQ("Discrete", fan.speedControlMethod());

  // Electric Power Minimum Flow Rate Fraction: Required Double
  EXPECT_TRUE(fan.setElectricPowerMinimumFlowRateFraction(0.775));
  EXPECT_EQ(0.775, fan.electricPowerMinimumFlowRateFraction());

  // Design Pressure Rise: Required Double
  EXPECT_TRUE(fan.setDesignPressureRise(121.5));
  EXPECT_EQ(121.5, fan.designPressureRise());

  // Motor Efficiency: Required Double
  EXPECT_TRUE(fan.setMotorEfficiency(0.54));
  EXPECT_EQ(0.54, fan.motorEfficiency());

  // Motor In Air Stream Fraction: Required Double
  EXPECT_TRUE(fan.setMotorInAirStreamFraction(0.87));
  EXPECT_EQ(0.87, fan.motorInAirStreamFraction());

  // Design Electric Power Consumption: Required Double
  fan.autosizeDesignElectricPowerConsumption();
  EXPECT_TRUE(fan.isDesignElectricPowerConsumptionAutosized());
  EXPECT_TRUE(fan.setDesignElectricPowerConsumption(3112.8));
  ASSERT_TRUE(fan.designElectricPowerConsumption());
  EXPECT_EQ(3112.8, fan.designElectricPowerConsumption().get());
  EXPECT_FALSE(fan.isDesignElectricPowerConsumptionAutosized());

  // Design Power Sizing Method: Required String
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("PowerPerFlow"));
  EXPECT_EQ("PowerPerFlow", fan.designPowerSizingMethod());
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure"));
  EXPECT_EQ("TotalEfficiencyAndPressure", fan.designPowerSizingMethod());
  EXPECT_FALSE(fan.setDesignPowerSizingMethod("BADENUM"));
  EXPECT_TRUE(fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure"));

  // Electric Power Per Unit Flow Rate: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRate(1254.0));
  EXPECT_EQ(1254.0, fan.electricPowerPerUnitFlowRate());

  // Electric Power Per Unit Flow Rate Per Unit Pressure: Required Double
  EXPECT_TRUE(fan.setElectricPowerPerUnitFlowRatePerUnitPressure(1.345));
  EXPECT_EQ(1.345, fan.electricPowerPerUnitFlowRatePerUnitPressure());

  // Fan Total Efficiency: Required Double
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.59));
  EXPECT_EQ(0.59, fan.fanTotalEfficiency());

  // Electric Power Function of Flow Fraction Curve Name: Optional Object
  CurveCubic fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));
  ASSERT_TRUE(fan.electricPowerFunctionofFlowFractionCurve());
  EXPECT_EQ(fanPowerFuncFlowCurve, fan.electricPowerFunctionofFlowFractionCurve().get());
  fan.resetElectricPowerFunctionofFlowFractionCurve();
  EXPECT_FALSE(fan.electricPowerFunctionofFlowFractionCurve());
  CurveBiquadratic badCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(badCurve));
  EXPECT_FALSE(fan.electricPowerFunctionofFlowFractionCurve());

  // Night Ventilation Mode Pressure Rise: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModePressureRise(356.0));
  ASSERT_TRUE(fan.nightVentilationModePressureRise());
  EXPECT_EQ(356.0, fan.nightVentilationModePressureRise().get());

  // Night Ventilation Mode Flow Fraction: Optional Double
  EXPECT_TRUE(fan.setNightVentilationModeFlowFraction(0.37));
  ASSERT_TRUE(fan.nightVentilationModeFlowFraction());
  EXPECT_EQ(0.37, fan.nightVentilationModeFlowFraction().get());

  // Motor Loss Zone Name: Optional Object
  ThermalZone z(m);
  EXPECT_TRUE(fan.setMotorLossZone(z));
  ASSERT_TRUE(fan.motorLossZone());
  EXPECT_EQ(z, fan.motorLossZone().get());

  // Motor Loss Radiative Fraction: Required Double
  EXPECT_TRUE(fan.setMotorLossRadiativeFraction(0.15));
  EXPECT_EQ(0.15, fan.motorLossRadiativeFraction());

  // End-Use Subcategory: Required String
  EXPECT_TRUE(fan.setEndUseSubcategory("My Fan"));
  EXPECT_EQ("My Fan", fan.endUseSubcategory());

  // Number of Speeds: Integer
  EXPECT_TRUE(fan.setNumberofSpeeds(3));
  EXPECT_EQ(3, fan.numberofSpeeds());

} // End of Getter_Setters test


TEST_F(ModelFixture,FanSystemModel_addToNode)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel testObject = FanSystemModel(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACWaterToAirHeatPump)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CoilHeatingWaterToAirHeatPumpEquationFit heatingCoil(m);
  CoilCoolingWaterToAirHeatPumpEquationFit coolingCoil(m);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ZoneHVACWaterToAirHeatPump zoneHVACWaterToAirHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACWaterToAirHeatPump.handle());
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACFourPipeFanCoil)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);

  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(m, s, fan, coolingCoil, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACFourPipeFanCoil.handle());
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACPackagedTerminalAirConditioner)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);

  ZoneHVACPackagedTerminalAirConditioner zoneHVACPackagedTerminalAirConditioner(m, s, fan, heatingCoil, coolingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalAirConditioner.handle());
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACPackagedTerminalHeatPump)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ZoneHVACPackagedTerminalHeatPump zoneHVACPackagedTerminalHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalHeatPump.handle());
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACUnitHeater)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CoilHeatingWater heatingCoil(m, s);

  ZoneHVACUnitHeater zoneHVACUnitHeater(m, s, fan, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACUnitHeater.handle());

}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACTerminalUnitVariableRefrigerantFlow)
{
  Model m;

  CoilCoolingDXVariableRefrigerantFlow coolingCoil(m);
  CoilHeatingDXVariableRefrigerantFlow heatingCoil(m);
  FanSystemModel fan(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow zoneHVACTerminalUnitVariableRefrigerantFlow(m, coolingCoil, heatingCoil, fan);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACTerminalUnitVariableRefrigerantFlow.handle());
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACBaseboardConvectiveElectric)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  ZoneHVACBaseboardConvectiveElectric zoneHVACBaseboardConvectiveElectric(m);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_FALSE(component);
}

TEST_F(ModelFixture,FanSystemModel_containingZoneHVACComponent_ZoneHVACLowTemperatureRadiantElectric)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(m, s, s);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_FALSE(component);
}

TEST_F(ModelFixture,FanSystemModel_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan = FanSystemModel(m);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ASSERT_ANY_THROW(AirLoopHVACUnitaryHeatPumpAirToAir airLoopHVACUnitaryHeatPumpAirToAir(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil));

}

TEST_F(ModelFixture, FanOnOff_Clone_SameModel)
{
  Model m;
  FanSystemModel fan = FanSystemModel(m);
  EXPECT_TRUE(fan.setDesignPressureRise(999.0));
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.99));
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(999.0));

  CurveExponent fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());
  EXPECT_EQ(1,  m.getModelObjects<FanSystemModel>().size());

  FanSystemModel fanClone = fan.clone(m).cast<FanSystemModel>();

  EXPECT_EQ(2,  m.getModelObjects<FanSystemModel>().size());

  powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());

  EXPECT_DOUBLE_EQ(999.0, fanClone.designPressureRise());
  EXPECT_DOUBLE_EQ(0.99, fanClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, fanClone.designMaximumAirFlowRate().get());
  ASSERT_TRUE(fanClone.electricPowerFunctionofFlowFractionCurve());
  EXPECT_EQ(fan.electricPowerFunctionofFlowFractionCurve().get(), fanClone.electricPowerFunctionofFlowFractionCurve().get());
}

TEST_F(ModelFixture, FanOnOff_Clone_OtherModel)
{
  Model m;
  FanSystemModel fan = FanSystemModel(m);
  EXPECT_TRUE(fan.setDesignPressureRise(999.0));
  EXPECT_TRUE(fan.setFanTotalEfficiency(0.99));
  EXPECT_TRUE(fan.setDesignMaximumAirFlowRate(999.0));

  CurveExponent fanPowerFuncFlowCurve(m);
  EXPECT_TRUE(fan.setElectricPowerFunctionofFlowFractionCurve(fanPowerFuncFlowCurve));

  Model m2;
  EXPECT_EQ(1, m.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(0, m2.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(0, m2.getModelObjects<CurveExponent>().size());

  FanSystemModel fanClone = fan.clone(m2).cast<FanSystemModel>();
  EXPECT_EQ(1, m.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(1, m2.getModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m2.getModelObjects<CurveExponent>().size());

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());

  std::vector<CurveExponent> powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves2.size());

  for (const auto& c: powerCurves) {
    EXPECT_TRUE(c.parent());
  }
  for (const auto& c: powerCurves2) {
    EXPECT_TRUE(c.parent());
  }

  EXPECT_DOUBLE_EQ(999.0, fanClone.designPressureRise());
  EXPECT_DOUBLE_EQ(0.99, fanClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, fanClone.designMaximumAirFlowRate().get());
  ASSERT_TRUE(fanClone.electricPowerFunctionofFlowFractionCurve());
  EXPECT_NE(fan.electricPowerFunctionofFlowFractionCurve().get().handle(), fanClone.electricPowerFunctionofFlowFractionCurve().get().handle());

}
