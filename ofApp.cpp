#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for(float radius = 150; radius <= 300; radius += 10){

		int deg_start = ofMap(ofNoise(radius * 0.0005 - ofGetFrameNum() * 0.01), 0, 1, -540, 540);
		deg_start = 0;
		this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 5, 50, deg_start, deg_start + 120, ofColor(239, 99, 99));
		this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 5, 50, deg_start + 180, deg_start + 300, ofColor(99, 99, 239));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.43);
	ofRotateY(ofGetFrameNum() * 0.65);

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end, ofColor color) {

	if (deg_start == deg_end) { return; }

	int index = face_target.getNumVertices();

	for (int deg = deg_start; deg <= deg_end; deg += 3) {

		auto face_index = face_target.getNumVertices();

		float angle_x = ofMap(ofNoise(this->noise_seed.x, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, PI * -1, PI * 1);
		auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
 
		float angle_y = ofMap(ofNoise(this->noise_seed.y, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, PI * -1, PI * 1);
		auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0)); 

		float angle_z = ofMap(ofNoise(this->noise_seed.z, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, PI * -3, PI * 3);
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + 3) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + 3) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + 3) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + 3) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + 3) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + 3) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + 3) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + 3) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex = location + glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);

		for (int i = 0; i < 8; i++) {

			face_target.addColor(color);
			frame_target.addColor(ofColor(39));
		}
	}

	face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 7);
	face_target.addIndex(index + 0); face_target.addIndex(index + 7); face_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	index = frame_target.getNumVertices() - 8;

	face_target.addIndex(index + 1); face_target.addIndex(index + 2); face_target.addIndex(index + 6);
	face_target.addIndex(index + 1); face_target.addIndex(index + 6); face_target.addIndex(index + 5);

	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}