//////////////////////////////////////////////////////////////////////////////
// This file is part of the Meteor-Remake                             		//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace meteor
{
	struct MeteorShader 
	{
		int32_t textureArg0;//textureId
		std::string textureArg1;//NORMAL 
		int32_t twoSideArg0;
		std::string blendArg0;
		std::string blendArg1;
		std::string blendArg2;
		float opaqueArg0;
	};

	struct CharacterMaterial
	{
		std::string texture;
		std::string option;
		glm::vec4 colorKey;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 emissive;
		float opacity;
		bool twoSide;
	};

	//GModel Binary File and GModel Geometry File
	struct GModelFile// gmb and gmc
	{
		int32_t texturesCount = 0;
		int32_t shaderCount = 0;
		int32_t sceneObjectsCount = 0;
		int32_t dummeyObjectsCount = 0;
		int32_t verticesCount = 0;
		int32_t facesCount = 0;
		std::string fileName;
		std::vector<MeteorShader> shaders;
		std::vector<std::string> texturesNames;
	};

	struct DesItem
	{
		std::string name;
		glm::vec3 pos = {};
		glm::quat quat{};
		bool useTextAnimation = false; //是否使用uv动画
		glm::vec2 textAnimation = {}; // uv参数
		std::vector<std::string> custom;
	};

	//GModel Description File
	struct DesFile
	{
		int32_t dummyCount = 0;
		int32_t objectCount = 0;
		std::unordered_map<std::string, DesItem> sceneItems;
	};

	struct FmcFrame
	{
		int32_t frameIdx = 0;
		std::vector<glm::vec3> pos;//pos for every objects
		std::vector<glm::quat> quat;//rotation for every objects
	};

	//# GModel Animation File
	struct FmcFile
	{
		int32_t fps = 0;
		int32_t frames = 0;
		int32_t scemeObjCount = 0;
		int32_t dummyObjCount = 0;
		std::vector<FmcFrame> fmcFrames;
	};

	struct MeteorSceneObject 
	{
		MeteorSceneObject(const std::string& name):name(name) {};
		std::string name;
		DesFile desFile; //SubMesh description;
		GModelFile gmbFile;// MeshObject
		std::shared_ptr<FmcFile> fmcFile;
	};

	//Character Skin
	struct SkcFile
	{
		SkcFile(const std::string& name) :name(name) {};
		std::string name;
		int32_t staticSkins = 0;
		int32_t dynmaicSkins = 0;
		std::string skin;
		std::vector<CharacterMaterial> materials;
	};

	struct MeteorBone 
	{
		std::string name;
		std::string parent;
		glm::vec3 offset;//local 
		glm::quat rotation;//local
		bool dummy;//unknown??
		uint32_t children;
	};

	struct BncFile//skeleton
	{
		BncFile(const std::string& name) :fileName(name) {};
		std::string fileName;

		std::vector<MeteorBone> bones;
		uint32_t boneSize = 0;
		uint32_t dummeySize = 0;
	};

	struct MeteorAnimationClip
	{
		int32_t flag;
		glm::vec3 bonePos;//相对位置,每一帧只有首骨骼有
		std::vector<glm::vec3> dummyPos;//虚拟对象相对位置
		std::vector<glm::quat> boneQuat;//相对旋转.
		std::vector<glm::quat> dummyQuat;//虚拟对象相对旋转
	};

	//Blend/Action
	struct PoseAction 
	{
		enum class Type
		{
			Blend,
			Action
		};
		Type type;
		int32_t start;
		int32_t end;
		float speed;
	};

	struct AttackInfo 
	{
		std::vector<std::string> bones;//攻击伤害盒
		int32_t poseIdx;//伤害由哪个动作赋予，由动作可以反向查找技能，以此算伤害
		int32_t start;
		int32_t end;
		int32_t attackType;//0普攻1破防
		int32_t checkFriend;
		float defenseValue;//防御僵硬
		float defenseMove;//防御时移动.
		float targetValue;//攻击僵硬
		float targetMove;//攻击时移动
		int32_t targetPose;//受击时播放动作
		int32_t targetPoseFront;//挨打倒地096
		int32_t targetPoseBack;//倒地前翻   099
		int32_t targetPoseLeft;//倒地右翻   098
		int32_t targetPoseRight;//倒地左翻  097
	};

	struct Pose
	{
		int32_t source;
		int32_t start;
		int32_t end;
		int32_t loopStart;
		int32_t loopEnd;
		int32_t effectType;
		std::string	effectID;
		std::vector<PoseAction> actions;
		int32_t link;
		std::vector<AttackInfo> attackInfos;
		
		struct
		{
			int32_t start;
			int32_t end;
			float time;
			glm::vec3 color;
		} drag;

		struct 
		{
			int32_t start;
			int32_t end;
			float time;
		}nextPose;
	};

	struct MeteorAnimation
	{
		std::vector<MeteorAnimationClip> clips;
		std::vector<Pose> poses;
	};
}