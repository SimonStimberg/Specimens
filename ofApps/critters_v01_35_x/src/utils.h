#pragma once
#include "ofMain.h"

namespace DrawUtils {

    // Filled triangle-strip ring for a CLOSED Catmull-Rom spline.
    // Use for: Breather, Pumper
    // Caller must set color before calling. Restores ofNoFill on exit.
    void thickClosedCurve(ofPolyline& spline, ofMesh& ring,
                          const vector<glm::vec2>& pts,
                          float halfWidth, int curveRes = 10);

    // Filled triangle-strip ribbon for an OPEN Catmull-Rom spline.
    // Use for: Intestine membranes, Neuron dendrites
    // roundCaps=true draws a filled circle at each endpoint.
    // Caller must set color before calling. Restores ofNoFill on exit.
    void thickOpenCurve(ofPolyline& spline, ofMesh& ring,
                        const vector<glm::vec2>& pts,
                        float halfWidth, int curveRes = 10,
                        bool roundCaps = false);

} // namespace DrawUtils