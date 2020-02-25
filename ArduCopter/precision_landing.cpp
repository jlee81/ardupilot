//
// functions to support precision landing
//

#include "Copter.h"

#if PRECISION_LANDING == ENABLED

void Copter::init_precland()
{
    copter.precland.init();
}

void Copter::update_precland()
{
    int32_t height_above_ground_cm = current_loc.alt;

    // use range finder altitude if it is valid, else try to get terrain alt
    if (rangefinder_alt_ok()) {
        height_above_ground_cm = rangefinder_state.alt_cm;
    } else if (terrain_use()) {
        if (!current_loc.get_alt_cm(Location_Class::ALT_FRAME_ABOVE_TERRAIN, height_above_ground_cm)) {
            height_above_ground_cm = current_loc.alt;
        }
    }

    copter.precland.update(height_above_ground_cm, rangefinder_alt_ok());

    static bool precland_flag = false;
    if (copter.precland.target_acquired() && !precland_flag )
    {
        gcs().send_text(MAV_SEVERITY_INFO, "IR Marker Target Acquired");        
    }
    else if (!copter.precland.target_acquired() && precland_flag )
    {
        gcs().send_text(MAV_SEVERITY_INFO, "IR Marker Target Lost");
    }
    precland_flag = copter.precland.target_acquired();
}
#endif
