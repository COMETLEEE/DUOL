#include "PhysicsSystemImpl.h"

/* Scene */
#include "../Scene/PhysicsSceneImpl.h"

/* Material */
#include "../Material/PhysicsMaterialImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/ObjectLayer.h"

namespace DUOLPhysics
{
	PhysicsSystem::PhysicsSystem() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
		Release();
	}

	bool PhysicsSystem::Init(const PhysicsSystemDesc& desc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			_impl->InitDefault();

			if (desc._usePvd == true)
				_impl->InitPvd();

			_impl->InitPhysics();

			if (desc.graphicsDevice != nullptr)
				_impl->InitCudaContextManager(PxCudaInteropMode::D3D11_INTEROP, desc.graphicsDevice);

			return true;
		}
		catch (const std::string& errStr)
		{
			Release();

			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			Release();

			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	void PhysicsSystem::AddLayer(const DUOLCommon::tstring& layer)
	{
		DUOLPhysics::ObjectLayerControl::AddLayer(layer);
	}

	bool PhysicsSystem::GetCollisionLayerState(const DUOLCommon::tstring& layer0, const DUOLCommon::tstring& layer1)
	{
		if (DUOLPhysics::ObjectLayerControl::_layerStates.contains(layer0) && DUOLPhysics::ObjectLayerControl::_layerStates.contains(layer1))
		{
			auto&& layerState0 = DUOLPhysics::ObjectLayerControl::GetLayerState(layer0);

			auto&& layerState1 = DUOLPhysics::ObjectLayerControl::GetLayerState(layer1);

			int layerMask0 = layerState0._layerMask;

			int layerNum1 = layerState1._layer;

			return (layerMask0 & layerNum1) != 0;
		}

		return false;
	}

	void PhysicsSystem::SetCollisionLayerState(const DUOLCommon::tstring& layer0, const DUOLCommon::tstring& layer1, bool state)
	{
		if (DUOLPhysics::ObjectLayerControl::_layerStates.contains(layer0) && DUOLPhysics::ObjectLayerControl::_layerStates.contains(layer1))
		{
			auto&& layerState0 = DUOLPhysics::ObjectLayerControl::GetLayerState(layer0);

			auto&& layerState1 = DUOLPhysics::ObjectLayerControl::GetLayerState(layer1);

			if (state)
			{
				layerState0._layerMask |= layerState1._layer;

				layerState1._layerMask |= layerState0._layer;
			}
			else
			{
				layerState0._layerMask &= ~(layerState1._layer);

				layerState1._layerMask &= ~(layerState0._layer);
			}
		}
	}

	unsigned int PhysicsSystem::GetLayerNumber(const DUOLCommon::tstring& layer)
	{
		return DUOLPhysics::ObjectLayerControl::_layerStates.at(layer)._layer;
	}

	bool PhysicsSystem::HasLayer(const DUOLCommon::tstring& layer)
	{
		return DUOLPhysics::ObjectLayerControl::_layerStates.contains(layer); 
	}

	void PhysicsSystem::GetLayer(unsigned layerNumber, DUOLCommon::tstring& outLayerName)
	{
		for (auto [key, value] : DUOLPhysics::ObjectLayerControl::_layerStates)
		{
			if (value._layer == layerNumber)
			{
				outLayerName = key;

				return;
			}
		}
	}

	unsigned PhysicsSystem::GetTotalLayerCount()
	{
		return static_cast<unsigned>(DUOLPhysics::ObjectLayerControl::_layerStates.size());
	}

	const std::unordered_map<DUOLCommon::tstring, unsigned>& PhysicsSystem::GetAllLayers()
	{
		return DUOLPhysics::ObjectLayerControl::_layers;
	}

	void PhysicsSystem::Release()
	{
		for (auto& iter : _scenes)
			iter.second->Release();

		_scenes.clear();

		for (auto& iter : _materials)
			iter.second->Release();

		_materials.clear();

		for (auto& iter : _shapes)
			iter.second->Release();

		_shapes.clear();

		if (_impl != nullptr)
			_impl->Release();
	}

	std::weak_ptr<PhysicsScene> PhysicsSystem::CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _scenes.find(keyName);

			if (result != _scenes.end())
				return result->second;

			auto newScene = std::make_shared<PhysicsScene>();
			newScene->_impl->Create(_impl->_physics, _impl->_cooking, _impl->_cpuDispatcher, _impl->_cudaContextManager, sceneDesc);
			
			_scenes[keyName] = newScene;

			return _scenes[keyName];
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	std::weak_ptr<PhysicsMaterial> PhysicsSystem::CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _materials.find(keyName);

			if (result != _materials.end())
				return result->second;

			auto newMaterial = std::make_shared<PhysicsMaterial>();
			newMaterial->_impl->Create(_impl->_physics, materialDesc);

			_materials[keyName] = newMaterial;

			return newMaterial;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	bool PhysicsSystem::DestroyScene(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _scenes.find(keyName);

			if (result == _scenes.end())
				return false;

			_scenes.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	bool PhysicsSystem::DestroyMaterial(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _materials.find(keyName);

			if (result == _materials.end())
				return false;

			_materials.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}

	bool PhysicsSystem::DestroyShape(const tstring& keyName)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _shapes.find(keyName);

			if (result == _shapes.end())
				return false;

			_shapes.erase(result);

			return true;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return false;
	}
}
