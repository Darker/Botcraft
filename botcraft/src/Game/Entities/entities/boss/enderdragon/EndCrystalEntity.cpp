#include "botcraft/Game/Entities/entities/boss/enderdragon/EndCrystalEntity.hpp"

namespace Botcraft
{
    const std::array<std::string, EndCrystalEntity::metadata_count> EndCrystalEntity::metadata_names{ {
        "data_beam_target",
        "data_show_bottom",
    } };

    EndCrystalEntity::EndCrystalEntity()
    {
        // Initialize all metadata with default values
        SetDataBeamTarget(std::optional<Position>());
        SetDataShowBottom(true);
    }

    EndCrystalEntity::~EndCrystalEntity()
    {

    }


    std::string EndCrystalEntity::GetName() const
    {
        return "EndCrystal";
    }

    EntityType EndCrystalEntity::GetType() const
    {
        return EntityType::EndCrystal;
    }

    double EndCrystalEntity::GetWidth() const
    {
        return 2.0;
    }

    double EndCrystalEntity::GetHeight() const
    {
        return 2.0;
    }


    std::string EndCrystalEntity::GetClassName()
    {
        return "EndCrystal";
    }

    EntityType EndCrystalEntity::GetClassType()
    {
        return EntityType::EndCrystal;
    }


    nlohmann::json EndCrystalEntity::Serialize() const
    {
        nlohmann::json output = Entity::Serialize();

        output["metadata"]["data_beam_target"] = GetDataBeamTarget() ? GetDataBeamTarget().value().Serialize() : nlohmann::json();
        output["metadata"]["data_show_bottom"] = GetDataShowBottom();

        return output;
    }


    void EndCrystalEntity::SetMetadataValue(const int index, const std::any& value)
    {
        if (index < hierarchy_metadata_count)
        {
            Entity::SetMetadataValue(index, value);
        }
        else if (index - hierarchy_metadata_count < metadata_count)
        {
            metadata[metadata_names[index - hierarchy_metadata_count]] = value;
        }
    }

    const std::optional<Position>& EndCrystalEntity::GetDataBeamTarget() const
    {
        return std::any_cast<const std::optional<Position>&>(metadata.at("data_beam_target"));
    }

    bool EndCrystalEntity::GetDataShowBottom() const
    {
        return std::any_cast<bool>(metadata.at("data_show_bottom"));
    }


    void EndCrystalEntity::SetDataBeamTarget(const std::optional<Position>& data_beam_target)
    {
        metadata["data_beam_target"] = data_beam_target;
    }

    void EndCrystalEntity::SetDataShowBottom(const bool data_show_bottom)
    {
        metadata["data_show_bottom"] = data_show_bottom;
    }

}
