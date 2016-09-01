/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** ElectricLoadCenterStorageSimple_Impl is a ElectricalStorage_Impl that is the implementation class for ElectricLoadCenterStorageSimple.*/
  class MODEL_API ElectricLoadCenterStorageSimple_Impl : public ElectricalStorage_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterStorageSimple_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    ElectricLoadCenterStorageSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    ElectricLoadCenterStorageSimple_Impl(const ElectricLoadCenterStorageSimple_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~ElectricLoadCenterStorageSimple_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{
    
    // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
    // Included in Base Class
    // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;
    bool isAvailabilityScheduleDefaulted() const;
    
    virtual boost::optional<ThermalZone> thermalZone() const override;

    double radiativeFractionforZoneHeatGains() const;
    bool isRadiativeFractionforZoneHeatGainsDefaulted() const;

    double nominalEnergeticEfficiencyforCharging() const;
    bool isNominalEnergeticEfficiencyforChargingDefaulted() const;

    // TODO: I've requested an .IDD change in EnergyPlus, to make this "Nominal Energetic Efficiency for Discharging"
    // TODO: https://github.com/NREL/EnergyPlus/issues/5730
    double nominalDischargingEnergeticEfficiency() const;
    bool isNominalDischargingEnergeticEfficiencyDefaulted() const;

    double maximumStorageCapacity() const;

    double maximumPowerforDischarging() const;

    double maximumPowerforCharging() const;

    double initialStateofCharge() const;
    bool isInitialStateofChargeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);
    void resetAvailabilitySchedule();
        
    // Override ElectricalStorage (Base class) methods
    virtual bool setThermalZone(const ThermalZone& thermalZone) override;
    virtual void resetThermalZone() override;

    bool setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains);
    void resetRadiativeFractionforZoneHeatGains();
    
    bool setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging);
    void resetNominalEnergeticEfficiencyforCharging();
    
    bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);
    void resetNominalDischargingEnergeticEfficiency();
    
    bool setMaximumStorageCapacity(double maximumStorageCapacity);
    
    bool setMaximumPowerforDischarging(double maximumPowerforDischarging);
    
    bool setMaximumPowerforCharging(double maximumPowerforCharging);
    
    bool setInitialStateofCharge(double initialStateofCharge);
    void resetInitialStateofCharge();

    //@}
    /** @name Other */
    //@{
      
    // TODO: Remove if not used. Don't think I need to override any of these
    //ModelObject clone(Model model) const override;

    //std::vector<openstudio::IdfObject> remove() override;

    //std::vector<ModelObject> children() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageSimple");
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP
