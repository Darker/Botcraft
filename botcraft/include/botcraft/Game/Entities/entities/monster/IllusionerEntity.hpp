#pragma once

#include "botcraft/Game/Entities/entities/monster/SpellcasterIllagerEntity.hpp"

namespace Botcraft
{
    class IllusionerEntity : public SpellcasterIllagerEntity
    {
    protected:
        static constexpr int metadata_count = 0;
        static constexpr int hierarchy_metadata_count = SpellcasterIllagerEntity::metadata_count + SpellcasterIllagerEntity::hierarchy_metadata_count;

    public:
        IllusionerEntity();
        virtual ~IllusionerEntity();

        // Object related stuff
        virtual std::string GetName() const override;
        virtual EntityType GetType() const override;
        virtual AABB GetCollider() const override;
        virtual double GetWidth() const override;
        virtual double GetHeight() const override;

        // Static stuff, for easier comparison
        static std::string GetClassName();
        static EntityType GetClassType();

    };
}
