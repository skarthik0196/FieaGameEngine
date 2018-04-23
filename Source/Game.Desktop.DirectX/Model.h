#pragma once

namespace Rendering
{
	class Mesh;
	class ModelMaterial;

	class Model
	{
	public:
		Model(const std::string& filename, bool flipUVs = false);
		Model(const Model& rhs) = default;
		Model(Model&& rhs) = default;
		virtual ~Model() = default;

		bool HasMaterials() const;
		bool HasMeshes() const;

		const std::vector<std::shared_ptr<Mesh>>& GetMeshes();
		const std::vector<std::shared_ptr<ModelMaterial>>& GetMaterials();

	private:
		void LoadModel(const std::string& fileName, bool flipUVs);

		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<ModelMaterial>> Materials;

	};
}