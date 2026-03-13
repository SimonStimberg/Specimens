#include "utils.h"

namespace DrawUtils {

void thickClosedCurve(ofPolyline& spline, ofMesh& ring,
                      const vector<glm::vec2>& pts,
                      float halfWidth, int curveRes) {
    int n = (int)pts.size();
    if (n < 3) return;

    spline.clear();
    spline.curveTo(glm::vec3(pts[n-1], 0.f), curveRes); // phantom prev
    for (int i = 0; i < n; i++)
        spline.curveTo(glm::vec3(pts[i],   0.f), curveRes);
    spline.curveTo(glm::vec3(pts[0],   0.f), curveRes); // close
    spline.curveTo(glm::vec3(pts[1],   0.f), curveRes); // C1 tangent

    const auto& sv = spline.getVertices();
    int nv = (int)sv.size();
    if (nv < 3) return;

    ring.clear();
    ring.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (int ii = 0; ii <= nv; ii++) {
        int idx  = ii % nv;
        int prev = (idx - 1 + nv) % nv;
        int next = (idx + 1) % nv;
        glm::vec3 t = sv[next] - sv[prev];
        float len = glm::length(t);
        if (len > 0.0001f) t /= len;
        glm::vec3 nm(-t.y, t.x, 0.f);
        ring.addVertex(sv[idx] + nm * halfWidth);
        ring.addVertex(sv[idx] - nm * halfWidth);
    }

    ofFill();
    ring.draw();
    ofNoFill();
}


void thickOpenCurve(ofPolyline& spline, ofMesh& ring,
                    const vector<glm::vec2>& pts,
                    float halfWidth, int curveRes, bool roundCaps) {
    int n = (int)pts.size();
    if (n < 2) return;

    spline.clear();
    spline.curveTo(glm::vec3(pts[0],   0.f), curveRes); // phantom prev
    for (int i = 0; i < n; i++)
        spline.curveTo(glm::vec3(pts[i],   0.f), curveRes);
    spline.curveTo(glm::vec3(pts[n-1], 0.f), curveRes); // phantom next

    const auto& sv = spline.getVertices();
    int nv = (int)sv.size();
    if (nv < 2) return;

    ring.clear();
    ring.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (int ii = 0; ii < nv; ii++) {
        glm::vec3 t;
        if      (ii == 0)      t = sv[1]    - sv[0];
        else if (ii == nv - 1) t = sv[nv-1] - sv[nv-2];
        else                   t = sv[ii+1] - sv[ii-1];
        float len = glm::length(t);
        if (len > 0.0001f) t /= len;
        glm::vec3 nm(-t.y, t.x, 0.f);
        ring.addVertex(sv[ii] + nm * halfWidth);
        ring.addVertex(sv[ii] - nm * halfWidth);
    }

    ofFill();
    ring.draw();
    if (roundCaps) {
        ofDrawCircle(sv[0].x,    sv[0].y,    halfWidth);
        ofDrawCircle(sv[nv-1].x, sv[nv-1].y, halfWidth);
    }
    ofNoFill();
}

} // namespace DrawUtils