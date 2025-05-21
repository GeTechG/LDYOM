#include <string>
#include <vector>

class ModelsManager {
  private:
	static std::vector<int> m_pedModels;
	static std::vector<std::string> m_pedSpecialModels;

  public:
	static const std::vector<int>& GetPedModels() { return m_pedModels; }

	static const std::vector<std::string>& GetPedSpecialModels() { return m_pedSpecialModels; }

	static void addPedModel(int modelId) { m_pedModels.push_back(modelId); }

	static void addPedSpecialModel(std::string modelName) { m_pedSpecialModels.push_back(modelName); }
};
