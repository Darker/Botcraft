#pragma once

#include "botcraft/Game/Entities/entities/vehicle/AbstractMinecartContainerEntity.hpp"

namespace Botcraft
{
    class MinecartChestEntity : public AbstractMinecartContainerEntity
    {
    protected:
        static constexpr int metadata_count = 0;
        static constexpr int hierarchy_metadata_count = AbstractMinecartContainerEntity::metadata_count + AbstractMinecartContainerEntity::hierarchy_metadata_count;

    public:
        MinecartChestEntity();
        virtual ~MinecartChestEntity();

        // Object related stuff
        virtual std::string GetName() const override;
        virtual EntityType GetType() const override;
        virtual double GetWidth() const override;
        virtual double GetHeight() const override;

        // Static stuff, for easier comparison
        static std::string GetClassName();
        static EntityType GetClassType();

    };
}
