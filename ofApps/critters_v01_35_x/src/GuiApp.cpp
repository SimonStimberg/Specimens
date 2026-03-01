#include "GuiApp.h"

#include "ofApp.h"
#include "ofxXmlSettings.h"

// #include <algorithm>
#include <array>
#include <sstream>

namespace {

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    const auto last = value.find_last_not_of(" \t\n\r");
    return value.substr(first, last - first + 1);
}

std::vector<std::string> splitAndTrim(const std::string& value) {
    std::vector<std::string> parts;
    std::stringstream stream(value);
    std::string token;
    while (std::getline(stream, token, ',')) {
        parts.push_back(trim(token));
    }
    return parts;
}

} // namespace

void GuiApp::setup(ofApp* app) {
    mainPtr = app;
    applyDefaults();

    const std::string defaultSimulationPreset = resolveFirstExistingPath({
        "../../../../Presets/Simulation/friesTV-physics_01.xml",
        "../../../../Presets/Simulation/lab30_01.xml",
        "bin/data/Presets/Simulation/SimulationPreset.xml",
        ofToDataPath("Presets/Simulation/SimulationPreset.xml", true)
    });
    loadPreset(defaultSimulationPreset, 1);

    const std::string defaultTubeCalibration = resolveFirstExistingPath({
        "../../../../Presets/TubeCalibrations/TinyScreen01.xml",
        "../../../../Presets/TubeCalibrations/24-09-18_PaceFestival.xml",
        "bin/data/Presets/TubeCalibrations/PhilipsCRT.xml",
        ofToDataPath("Presets/TubeCalibrations/PhilipsCRT.xml", true)
    });
    loadPreset(defaultTubeCalibration, 1);

    publishValues();
}

bool GuiApp::loadPreset(const std::string& filePath, int panel) {
    if (filePath.empty()) {
        ofLogWarning("GuiApp") << "Skipping empty preset path";
        return false;
    }

    ofFile presetFile(filePath);
    if (!presetFile.exists()) {
        ofLogWarning("GuiApp") << "Preset file not found: " << filePath;
        return false;
    }

    const std::string extension = ofToUpper(presetFile.getExtension());
    if (extension != "XML") {
        ofLogWarning("GuiApp") << "Unsupported preset extension: " << filePath;
        return false;
    }

    bool loaded = false;
    if (panel == 1) {
        loaded = loadSimulationPreset(filePath);
        loaded = loadTubeCalibrationPreset(filePath) || loaded;
    } else if (panel == 2) {
        ofLogNotice("GuiApp") << "Synth preset loading is currently disabled: " << filePath;
    }

    if (loaded) {
        publishValues();
    }

    return loaded;
}

void GuiApp::applyDefaults() {
    tuneFriction = 0.97f;
    tuneRepulsionThresh = 18.0f;
    tuneRepulsionForce = 0.0f;
    tuneFlatThresh = 7.0f;
    tuneFlatDistance = 20.0f;
    tuneFlatLimitForce = -3.0f;
    tuneSpringLength = 22.0f;
    tuneSpringElasticity = -5.0f;
    tuneSpringLimitForce = 0.0f;
    switchOscillation = false;
    tuneExpansionForce = 0.0f;
    tuneExpansionRadius = 3.0f;
    tuneOscillationAmount = 0.1f;
    tuneIntrusionThresh = 62.0f;
    tuneIntrusionForce = -1.0f;
    switchConnections = false;
    switchKinectCalibration = false;
    switchScreenMask = false;
    tunePressureTest = 1.0f;

    tuneBreatherExpansionForce = 0.0f;
    tuneBreatherExpansionRadius = 25.0f;
    tuneBreatherOscillationAmount = 0.1f;
    tuneJointLength = 22.0f;
    tuneJointElasticity = -5.0f;
    tuneJointLimitForce = 0.0f;

    tuneMembraneLength = 22.0f;
    tuneMembraneElasticity = -5.0f;
    tuneMembraneLimitForce = 0.0f;
    tuneStructureLength = 22.0f;
    tuneStructureElasticity = -5.0f;
    tuneStructureLimitForce = 0.0f;
    membraneColor = ofColor(40, 40, 130, 255);

    tuneDendriteLength = 22.0f;
    tuneDendriteElasticity = -5.0f;
    tuneDendriteLimitForce = 0.0f;

    cellNextGrowth = glm::vec2(100.0f, 0.5f);
    cellFreqMultiplier = 1.0f;

    pumperSyncDistance = 100;

    neuronGrowSpeed = 500;
    neuronSignalInterval = 2000;
    neuronSyncDistance = 100;

    intestineGrowInterval = 700;
    intestineDigestionInterval = 4000;
    intestineDigestionSpeed = 300;
    intestineSyncDistance = 150;

    maskChanged = false;
    debugMode = false;
}

void GuiApp::publishValues() {
    values.clear();

    values["tuneFriction"] = tuneFriction;
    values["tuneRepulsionThresh"] = tuneRepulsionThresh;
    values["tuneRepulsionForce"] = tuneRepulsionForce;
    values["tuneFlatThresh"] = tuneFlatThresh;
    values["tuneFlatDistance"] = tuneFlatDistance;
    values["tuneFlatLimitForce"] = tuneFlatLimitForce;
    values["tuneSpringLength"] = tuneSpringLength;
    values["tuneSpringElasticity"] = tuneSpringElasticity;
    values["tuneSpringLimitForce"] = tuneSpringLimitForce;
    values["switchOscillation"] = switchOscillation;
    values["tuneExpansionForce"] = tuneExpansionForce;
    values["tuneExpansionRadius"] = tuneExpansionRadius;
    values["tuneOscillationAmount"] = tuneOscillationAmount;
    values["tuneIntrusionThresh"] = tuneIntrusionThresh;
    values["tuneIntrusionForce"] = tuneIntrusionForce;
    values["switchConnections"] = switchConnections;
    values["switchKinectCalibration"] = switchKinectCalibration;
    values["switchScreenMask"] = switchScreenMask;
    values["tunePressureTest"] = tunePressureTest;

    values["tuneBreatherExpansionForce"] = tuneBreatherExpansionForce;
    values["tuneBreatherExpansionRadius"] = tuneBreatherExpansionRadius;
    values["tuneBreatherOscillationAmount"] = tuneBreatherOscillationAmount;
    values["tuneJointLength"] = tuneJointLength;
    values["tuneJointElasticity"] = tuneJointElasticity;
    values["tuneJointLimitForce"] = tuneJointLimitForce;

    values["tuneMembraneLength"] = tuneMembraneLength;
    values["tuneMembraneElasticity"] = tuneMembraneElasticity;
    values["tuneMembraneLimitForce"] = tuneMembraneLimitForce;
    values["tuneStructureLength"] = tuneStructureLength;
    values["tuneStructureElasticity"] = tuneStructureElasticity;
    values["tuneStructureLimitForce"] = tuneStructureLimitForce;
    values["membraneColor"] = membraneColor;

    values["tuneDendriteLength"] = tuneDendriteLength;
    values["tuneDendriteElasticity"] = tuneDendriteElasticity;
    values["tuneDendriteLimitForce"] = tuneDendriteLimitForce;

    values["cellNextGrowth"] = cellNextGrowth;
    values["cellFreqMultiplier"] = cellFreqMultiplier;

    values["pumperSyncDistance"] = pumperSyncDistance;

    values["neuronGrowSpeed"] = neuronGrowSpeed;
    values["neuronSignalInterval"] = neuronSignalInterval;
    values["neuronSyncDistance"] = neuronSyncDistance;

    values["intestineGrowInterval"] = intestineGrowInterval;
    values["intestineDigestionInterval"] = intestineDigestionInterval;
    values["intestineDigestionSpeed"] = intestineDigestionSpeed;
    values["intestineSyncDistance"] = intestineSyncDistance;

    values["maskChanged"] = maskChanged;
    values["debugMode"] = debugMode;
}

bool GuiApp::loadSimulationPreset(const std::string& filePath) {
    ofxXmlSettings xml;
    if (!xml.load(filePath)) {
        return false;
    }

    auto getFloat = [&xml](const std::string& tag, float currentValue) {
        return xml.tagExists(tag) ? xml.getValue(tag, currentValue) : currentValue;
    };

    auto getInt = [&xml](const std::string& tag, int currentValue) {
        return xml.tagExists(tag) ? xml.getValue(tag, currentValue) : currentValue;
    };

    auto getBool = [&xml](const std::string& tag, bool currentValue) {
        if (!xml.tagExists(tag)) return currentValue;
        return parseBool(xml.getValue(tag, std::string(currentValue ? "1" : "0")), currentValue);
    };

    auto getVec2 = [&xml](const std::string& tag, const glm::vec2& currentValue) {
        if (!xml.tagExists(tag)) return currentValue;
        return parseVec2(xml.getValue(tag, std::string("")), currentValue);
    };

    auto getColor = [&xml](const std::string& tag, const ofColor& currentValue) {
        if (!xml.tagExists(tag)) return currentValue;
        return parseColor(xml.getValue(tag, std::string("")), currentValue);
    };

    tuneFriction = getFloat("group:Tune_Forces:Friction", tuneFriction);
    tuneRepulsionThresh = getFloat("group:Tune_Forces:Repulsion_Radius", tuneRepulsionThresh);
    tuneRepulsionForce = getFloat("group:Tune_Forces:Tune_Repulsion_Force", tuneRepulsionForce);
    tuneFlatThresh = getFloat("group:Tune_Forces:Flattening_min_Thresh", tuneFlatThresh);
    tuneFlatDistance = getFloat("group:Tune_Forces:Flattening_Distance", tuneFlatDistance);
    tuneFlatLimitForce = getFloat("group:Tune_Forces:Tune_Flattening_Force_Limit", tuneFlatLimitForce);
    tuneSpringLength = getFloat("group:Tune_Forces:Spring_Length", tuneSpringLength);
    tuneSpringElasticity = getFloat("group:Tune_Forces:Tune_Spring_Elasticity", tuneSpringElasticity);
    tuneSpringLimitForce = getFloat("group:Tune_Forces:Tune_Spring_Force_limit", tuneSpringLimitForce);
    switchOscillation = getBool("group:Tune_Forces:Switch_breathing", switchOscillation);
    tuneExpansionForce = getFloat("group:Tune_Forces:Tune_Expansion_Force", tuneExpansionForce);
    tuneExpansionRadius = getFloat("group:Tune_Forces:Tune_Expansion_Radius_Factor", tuneExpansionRadius);
    tuneOscillationAmount = getFloat("group:Tune_Forces:Tune_Oscillation_Amount", tuneOscillationAmount);
    tuneIntrusionThresh = getFloat("group:Tune_Forces:Interaction_Repul_Radius", tuneIntrusionThresh);
    tuneIntrusionForce = getFloat("group:Tune_Forces:Interaction_Repul_Force", tuneIntrusionForce);
    switchConnections = getBool("group:Tune_Forces:Switch_connections", switchConnections);

    tuneBreatherExpansionForce = getFloat("group:Tune_Breather_Shape:Tune_Expansion_Force", tuneBreatherExpansionForce);
    tuneBreatherExpansionRadius = getFloat("group:Tune_Breather_Shape:Tune_Expansion_Radius_Factor", tuneBreatherExpansionRadius);
    tuneJointLength = getFloat("group:Tune_Breather_Shape:Joint_Length", tuneJointLength);
    tuneJointElasticity = getFloat("group:Tune_Breather_Shape:Joint_Elasticity", tuneJointElasticity);
    tuneJointLimitForce = getFloat("group:Tune_Breather_Shape:Joint_Force_limit", tuneJointLimitForce);

    tuneMembraneLength = getFloat("group:Tune_Intestine_Shape:Membrane_Length", tuneMembraneLength);
    tuneMembraneElasticity = getFloat("group:Tune_Intestine_Shape:Membrane_Elasticity", tuneMembraneElasticity);
    tuneMembraneLimitForce = getFloat("group:Tune_Intestine_Shape:Membrane_Force_limit", tuneMembraneLimitForce);
    tuneStructureLength = getFloat("group:Tune_Intestine_Shape:Structure_Length", tuneStructureLength);
    tuneStructureElasticity = getFloat("group:Tune_Intestine_Shape:Structure_Elasticity", tuneStructureElasticity);
    tuneStructureLimitForce = getFloat("group:Tune_Intestine_Shape:Structure_Force_limit", tuneStructureLimitForce);
    membraneColor = getColor("group:Tune_Intestine_Shape:Membrane_Color", membraneColor);

    tuneDendriteLength = getFloat("group:Tune_Neuron_Shape:Dendrite_Length", tuneDendriteLength);
    tuneDendriteElasticity = getFloat("group:Tune_Neuron_Shape:Dendrite_Elasticity", tuneDendriteElasticity);
    tuneDendriteLimitForce = getFloat("group:Tune_Neuron_Shape:Dendrite_Force_limit", tuneDendriteLimitForce);

    switchKinectCalibration = getBool("group:General_Settings:Kinect_Calibration_Mode", switchKinectCalibration);
    switchScreenMask = getBool("group:General_Settings:Screen_Mask_Calibration_Mode", switchScreenMask);

    cellNextGrowth = getVec2("group:Timings:Breathers:Cell_Next_Growth", cellNextGrowth);
    cellFreqMultiplier = getFloat("group:Timings:Breathers:Cell_Freq_Multiplier", cellFreqMultiplier);

    pumperSyncDistance = getInt("group:Timings:Pumpers:Pumper_Sync_Distance", pumperSyncDistance);

    neuronGrowSpeed = getInt("group:Timings:Neurons:Neuron_Grow_Speed", neuronGrowSpeed);
    neuronSignalInterval = getInt("group:Timings:Neurons:Neuron_Signal_Inteval", neuronSignalInterval);
    neuronSyncDistance = getInt("group:Timings:Neurons:Neuron_Sync_Distance", neuronSyncDistance);

    intestineGrowInterval = getInt("group:Timings:Intestines:Intestine_Grow_Interval", intestineGrowInterval);
    intestineDigestionInterval = getInt("group:Timings:Intestines:Intestine_Digestion_Interval", intestineDigestionInterval);
    intestineDigestionSpeed = getInt("group:Timings:Intestines:Intestine_Digestion_Speed", intestineDigestionSpeed);
    intestineSyncDistance = getInt("group:Timings:Intestines:Intestine_Sync_Distance", intestineSyncDistance);

    tunePressureTest = getFloat("group:Tune_Forces:Pressure_Test", tunePressureTest);

    return true;
}

bool GuiApp::loadTubeCalibrationPreset(const std::string& filePath) {
    if (mainPtr == nullptr) {
        return false;
    }

    ofxXmlSettings xml;
    if (!xml.load(filePath)) {
        return false;
    }

    if (!xml.tagExists("group:Tune_Canvas")) {
        return false;
    }

    bool foundAny = false;

    for (int i = 0; i < mainPtr->numScreens; ++i) {
        const std::string prefix = "group:Tune_Canvas:Shape_Tube_" + ofToString(i) + ":";

        if (!xml.tagExists(prefix + "Canvas_Width")) {
            continue;
        }

        mainPtr->molSystem[i].tuneCanvasWidth = xml.getValue(prefix + "Canvas_Width", (float)mainPtr->molSystem[i].tuneCanvasWidth);
        mainPtr->molSystem[i].tuneCanvasHeight = xml.getValue(prefix + "Canvas_Height", (float)mainPtr->molSystem[i].tuneCanvasHeight);
        mainPtr->molSystem[i].tuneVerticalBow = xml.getValue(prefix + "Vertical_Bow", (float)mainPtr->molSystem[i].tuneVerticalBow);
        mainPtr->molSystem[i].tuneHorizontalBow = xml.getValue(prefix + "Horizontal_Bow", (float)mainPtr->molSystem[i].tuneHorizontalBow);
        mainPtr->molSystem[i].tuneEdges = xml.getValue(prefix + "Smooth_Edges", (float)mainPtr->molSystem[i].tuneEdges);
        mainPtr->molSystem[i].tuneXpos = xml.getValue(prefix + "X_Position", (float)mainPtr->molSystem[i].tuneXpos);
        mainPtr->molSystem[i].tuneYpos = xml.getValue(prefix + "Y_Position", (float)mainPtr->molSystem[i].tuneYpos);

        if (xml.tagExists(prefix + "Rotation")) {
            mainPtr->molSystem[i].tuneRotation = xml.getValue(prefix + "Rotation", (float)mainPtr->molSystem[i].tuneRotation);
        }

        foundAny = true;
    }

    maskChanged = foundAny;
    return foundAny;
}

bool GuiApp::parseBool(const std::string& value, bool fallback) {
    const std::string lower = ofToLower(trim(value));
    if (lower == "1" || lower == "true" || lower == "yes") return true;
    if (lower == "0" || lower == "false" || lower == "no") return false;
    return fallback;
}

glm::vec2 GuiApp::parseVec2(const std::string& value, const glm::vec2& fallback) {
    const std::vector<std::string> parts = splitAndTrim(value);
    if (parts.size() < 2) return fallback;

    try {
        return glm::vec2(std::stof(parts[0]), std::stof(parts[1]));
    } catch (...) {
        return fallback;
    }
}

ofColor GuiApp::parseColor(const std::string& value, const ofColor& fallback) {
    const std::vector<std::string> parts = splitAndTrim(value);
    if (parts.size() < 3) return fallback;

    try {
        const int r = ofClamp(std::stoi(parts[0]), 0, 255);
        const int g = ofClamp(std::stoi(parts[1]), 0, 255);
        const int b = ofClamp(std::stoi(parts[2]), 0, 255);
        const int a = (parts.size() > 3) ? ofClamp(std::stoi(parts[3]), 0, 255) : 255;
        return ofColor(r, g, b, a);
    } catch (...) {
        return fallback;
    }
}

std::string GuiApp::resolveFirstExistingPath(const std::vector<std::string>& candidates) {
    for (const std::string& candidate : candidates) {
        if (candidate.empty()) continue;
        ofFile file(candidate);
        if (file.exists()) {
            return candidate;
        }
    }

    return "";
}
