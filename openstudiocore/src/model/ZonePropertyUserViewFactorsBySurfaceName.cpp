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

#include <vector>
#include <string>
#include "ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
/* #include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp" */

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

ViewFactorData::ViewFactorData(const PlanarSurface& fromSurface, const PlanarSurface& toSurface, double viewFactor)
  : m_from_surface(fromInternalMass), m_to_surface(toSubSurface), m_view_factor(viewFactor) {
    
  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor data, view factor of " << m_view_factor << " more than 1");
  }
}

PlanarSurface ViewFactorData::fromSurface() const {
  return m_from_surface;
}

PlanarSurface ViewFactorData::toSurface() const {
  return m_to_surface;
}

double ViewFactorData::viewFactor() const {
  return m_view_factor;
}

std::ostream& operator<< (std::ostream& out, const openstudio::model::ViewFactorData& viewFactor) {
  out << "(fromSurface " << viewFactor.fromSurface().nameString() << ", " << "toSurface " << viewFactor.toSurface().nameString() << ", " << viewFactor.viewFactor() << ")";
  return out;
}

namespace detail {

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
  }

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
  }

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const ZonePropertyUserViewFactorsBySurfaceName_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZonePropertyUserViewFactorsBySurfaceName_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ZonePropertyUserViewFactorsBySurfaceName_Impl::iddObjectType() const {
    return ZonePropertyUserViewFactorsBySurfaceName::iddObjectType();
  }

  ThermalZone ZonePropertyUserViewFactorsBySurfaceName_Impl::thermalZone() const
  {
    boost::optional<ThermalZone> thermalZone = getObject<ModelObject>.getModelObjectTarget<ThermalZone>(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName);
    OS_ASSERT(thermalZone);
    return thermalZone.get();
  }
  
  unsigned int ZonePropertyUserViewFactorsBySurfaceName_Impl::numberofViewFactors() const {
    return numExtensibleGroups();
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const ViewFactorData& viewFactor) {
    bool result;
    
    // Push an extensible group
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool fromSurface = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName, viewFactor.fromSurface().handle());
    bool toSurface = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName, viewFactor.toSurface().handle());
    bool value = eg.setDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor, viewFactor.viewFactor());
    if (fromSurface && toSurface && value) {
      result = true;
    } else {
      // Something went wrong
      // So erase the new extensible group
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      result = false;
    }
    return result;
  }
  
  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const PlanarSurface& fromSurface, const PlanarSurface& toSurface, double value) {
    ViewFactorData viewFactor(fromSurface, toSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::removeViewFactor(unsigned groupIndex) {
    bool result;
    
    unsigned int num = numberofViewFactors();
    if (groupIndex < num) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      result = true;
    } else {
      result = false;
    }
    return result;
  }

  void ZonePropertyUserViewFactorsBySurfaceName_Impl::removeAllViewFactors() {
    getObject<ModelObject>().clearExtensibleGroups();
  }
  
  std::vector<ViewFactorData> ZonePropertyUserViewFactorsBySurfaceName_Impl::viewFactors() const {
    std::vector<ViewFactorData> result;
    
    std::vector<IdfExtensibleGroup> groups = extensibleGroups();
    
    for (const auto & group : groups) {
      boost::optional<WorkspaceObject> wo1 = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName);
      boost::optional<WorkspaceObject> wo2 = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName);
      boost::optional<double> value = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor);
      if (wo1->optionalCast<PlanarSurface>() && wo2->optionalCast<PlanarSurface>()) {
        boost::optional<PlanarSurface> fromSurface = wo1->optionalCast<PlanarSurface>();
        boost::optional<PlanarSurface> toSurface = wo2->optionalCast<PlanarSurface>();
        if (fromSurface && toSurface && value) {
          ViewFactorData viewFactor(from.get(), to.get(), value.get());
          result.push_back(viewFactor);
        }
      }
    }
    
    return result;
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactors(const std::vector<ViewFactorData> &viewFactors) {
    unsigned int num = numberofViewFactors();

    for (const ViewFactorData& viewFactor : viewFactors) {
      addViewFactor(viewFactor);
    }
    return true;    
  }

} // detail

ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(ThermalZone& thermalZone)
  : ModelObject(ZonePropertyUserViewFactorsBySurfaceName::iddObjectType(), thermalZone.model())
{
  OS_ASSERT(getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>());

  bool ok = true;
  OS_ASSERT(ok);

  ok = setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName, thermalZone.handle());
  OS_ASSERT(ok);
}

IddObjectType ZonePropertyUserViewFactorsBySurfaceName::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneProperty_UserViewFactors_BySurfaceName);
}

ThermalZone> ZonePropertyUserViewFactorsBySurfaceName::thermalZone() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->thermalZone();
}

unsigned int ZonePropertyUserViewFactorsBySurfaceName::numberofViewFactors() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->numberofViewFactors();
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const ViewFactor& viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const PlanarSurface& fromSurface, PlanarSurface& toSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toSurface, viewFactor);
}

void ZonePropertyUserViewFactorsBySurfaceName::removeViewFactor(int groupIndex) {
  getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeViewFactor(groupIndex);
}

void ZonePropertyUserViewFactorsBySurfaceName::removeAllViewFactors() {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeAllViewFactors();
}

std::vector<ViewFactorData> ZonePropertyUserViewFactorsBySurfaceName::viewFactors() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->viewFactors();
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactors(const std::vector<ViewFactorData> &viewFactors) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactors(viewFactors);
}

/// @cond
ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(std::shared_ptr<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio