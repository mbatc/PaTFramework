#pragma once

#include "CLog.h"
#include "CStringIDList.h"

#define ATD_IMAGE_JPG 0x01
#define ATD_IMAGE_TIF 0x02
#define ATD_IMAGE_PNG 0x03
#define ATD_IMAGE_BMP 0x04

#define ATD_TYPE_MESH		"_mesh_"
#define ATD_TYPE_AUDIO		"_audio_"
#define ATD_TYPE_TEXTURE	"_texture_"
#define ATD_TYPE_SCENE		"_scene_"
#define ATD_TYPE_GMOBJ		"_gameobject_"

#define IMPORT_NO_ERR					0x00
#define IMPORT_ERR_INVALID_FILE_HEADER  0x01

class CImporter
{
private:
	CImporter() { init_image_types(); }
	static CImporter* _instance;
public:
	~CImporter();
	static CImporter* get_instance()
	{
		if (!_instance)
		{
			_instance = new CImporter;
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CImporter instance created (ptr=%p)", _instance);
		}
		return _instance;
	}

	unsigned int import(std::string file);
	
	void do_force_import(bool b) { _force_import = b; }
private:
	unsigned int import_mesh(std::string file);
	unsigned int import_texture(std::string file);
	unsigned int import_audio(std::string file);
	unsigned int import_scene(std::string file);
	unsigned int import_gameobject(std::string file);

	void init_image_types();

	std::vector<std::string> m_image_type;
	bool _force_import;
};