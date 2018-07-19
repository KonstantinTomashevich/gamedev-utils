#include "DoubledCoordsHexGrid.hpp"
#include <UniversalException/UniversalException.hpp>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <queue>

const std::vector <std::pair <int, int> > DoubledCoordsHexGrid::DoubleWidthAvailableMoves = {{0,  2},
                                                                                             {1,  1},
                                                                                             {-1, 1},
                                                                                             {0,  -2},
                                                                                             {-1, -1},
                                                                                             {1,  -1}};

const std::vector <std::pair <int, int> > DoubledCoordsHexGrid::DoubleHeightAvailableMoves = {{2,  0},
                                                                                              {-1, 1},
                                                                                              {1,  1},
                                                                                              {-2, 0},
                                                                                              {1,  -1},
                                                                                              {-1, -1}};

float DoubledCoordsHexGrid::MedianCostCalculator (float from, float to)
{
    return (from + to) / 2.0f;
}

DoubledCoordsHexGrid::DoubledCoordsHexGridNeighborsIterator::DoubledCoordsHexGridNeighborsIterator (
        const DoubledCoordsHexGrid *ownerGrid, unsigned int currentRow,
        unsigned int currentCol, bool ignoreImpassable) :

        ownerGrid_ (ownerGrid),
        moveIndex_ (0),
        currentRow_ (currentRow),
        currentCol_ (currentCol),
        ignoreImpassable_ (ignoreImpassable)
{

}

void DoubledCoordsHexGrid::DoubledCoordsHexGridNeighborsIterator::Increment ()
{
    ++moveIndex_;
}

VertexOutcomingConnection DoubledCoordsHexGrid::DoubledCoordsHexGridNeighborsIterator::Get ()
{
    try
    {
        auto movesPtr = ownerGrid_->GetAvailableMovesList ();
        unsigned int newRow = currentRow_ + movesPtr->at (moveIndex_).first;
        unsigned int newCol = currentCol_ + movesPtr->at (moveIndex_).second;

        unsigned int currentHash = ownerGrid_->EncodeCellPosition (currentRow_, currentCol_);
        unsigned int newHash = ownerGrid_->EncodeCellPosition (newRow, newCol);

        return {(int) newHash, ownerGrid_->GetCostBetween (currentHash, newHash)};
    }
    catch (std::out_of_range &exception)
    {
        throw UniversalException <DoubledCoordsHexGridNeighborsIterator::Invalid> (
                std::string ("DoubledCoordsHexGridNeighborsIterator: move index overflow, it is ") +
                        std::to_string (moveIndex_) + std::string (", but moves count is ") +
                        std::to_string (ownerGrid_->GetAvailableMovesList ()->size ()) + std::string ("!")
        );
    }
    catch (AnyUniversalException &exception)
    {
        throw UniversalException <DoubledCoordsHexGridNeighborsIterator::Invalid> (
                std::string ("DoubledCoordsHexGridNeighborsIterator: due to invalidation, another exception is caught,"
                        "its' text: ") + exception.GetException ()
        );
    }
}

bool DoubledCoordsHexGrid::DoubledCoordsHexGridNeighborsIterator::Valid ()
{
    auto movesPtr = ownerGrid_->GetAvailableMovesList ();
    unsigned int currentHash = ownerGrid_->EncodeCellPosition (currentRow_, currentCol_);

    unsigned int newRow = 0;
    unsigned int newCol = 0;

    while (moveIndex_ < movesPtr->size ())
    {
        newRow = currentRow_ + movesPtr->at (moveIndex_).first;
        newCol = currentCol_ + movesPtr->at (moveIndex_).second;
        unsigned int newHash = ownerGrid_->EncodeCellPosition (newRow, newCol);

        if (newRow < ownerGrid_->GetMaxRow () && newCol < ownerGrid_->GetMaxCol () &&
                (!ignoreImpassable_ || ownerGrid_->GetCostBetween (currentHash, newHash) >= 0.0f))
        {
            break;
        }
        else
        {
            ++moveIndex_;
        }
    }
    return moveIndex_ < movesPtr->size ();
}

DoubledCoordsHexGrid::DoubledCoordsHexGrid (DoubledCoordsHexGrid::Type type, unsigned int maxRow, unsigned int maxCol,
        float hexRadius, const DoubledCoordsHexGrid::CostCalculator &costCalculator) :
        type_ (type),
        maxRow_ (maxRow),
        maxCol_ (maxCol),
        hexRadius_ (hexRadius),
        costCalculator_ (costCalculator)
{

}

SimpleIterator <VertexOutcomingConnection> *DoubledCoordsHexGrid::GetOutcomingConnections (int vertex) const
{
    unsigned int row, col;
    DecodeCellPosition (vertex, row, col);
    return new DoubledCoordsHexGridNeighborsIterator (this, row, col, true);
}

SimpleIterator <VertexOutcomingConnection> *
DoubledCoordsHexGrid::GetOutcomingConnections (unsigned int vertex, bool ignoreImpassable) const
{
    unsigned int row, col;
    DecodeCellPosition (vertex, row, col);
    return new DoubledCoordsHexGridNeighborsIterator (this, row, col, ignoreImpassable);
}

float DoubledCoordsHexGrid::HeuristicDistance (int beginVertex, int endVertex) const
{
    unsigned int bRow, bCol, eRow, eCol;
    DecodeCellPosition (beginVertex, bRow, bCol);
    DecodeCellPosition (endVertex, eRow, eCol);

    int dRow = std::abs ((int) bRow - (int) eRow);
    int dCol = std::abs ((int) bCol - (int) eCol);
    return type_ == Type::DoubleWidth ?
            dRow + std::max (0, (dCol - dRow) / 2) :
            dCol + std::max (0, (dRow - dCol) / 2);
}

unsigned int DoubledCoordsHexGrid::EncodeCellPosition (unsigned int row, unsigned int col) const
{
    unsigned int multiplier = std::max (maxRow_, maxCol_);
    return row * multiplier + col;
}

void DoubledCoordsHexGrid::DecodeCellPosition (unsigned int vertex, unsigned int &row, unsigned int &col) const
{
    unsigned int multiplier = std::max (maxRow_, maxCol_);
    col = vertex % multiplier;
    row = vertex / multiplier;
}

const std::vector <std::pair <int, int> > *DoubledCoordsHexGrid::GetAvailableMovesList () const
{
    return type_ == Type::DoubleWidth ? &DoubleWidthAvailableMoves : &DoubleHeightAvailableMoves;
}

float DoubledCoordsHexGrid::GetRectWidth () const
{
    return static_cast <float> (type_ == Type::DoubleWidth ?
            sqrt (3) * 0.5f * hexRadius_ * (maxCol_ + 1) :
            1.5f * hexRadius_ * maxCol_ + 0.75f * hexRadius_);
}

float DoubledCoordsHexGrid::GetRectHeight () const
{
    return static_cast <float> (type_ == Type::DoubleHeight ?
            sqrt (3) * 0.5f * hexRadius_ * (maxRow_ + 1) :
            1.5f * hexRadius_ * maxRow_ + 0.75f * hexRadius_);
}

float DoubledCoordsHexGrid::GetCostBetween (unsigned int fromHash, unsigned int toHash) const
{
    try
    {
        float fromCost = costModifiers_.at (fromHash);
        float toCost = costModifiers_.at (toHash);

        if (fromCost < 0.0f || toCost < 0.0f)
        {
            return -1.0f;
        }

        return costCalculator_ (fromCost, toCost);
    }
    catch (std::out_of_range &exception)
    {
        throw UniversalException <DoubledCoordsHexGrid::UnknownHash> (
                std::string (
                        "DoubledCoordsHexGrid: unknown hash exception while calling std methods. STD exception: ") +
                        exception.what ()
        );
    }
}

std::vector <unsigned int> DoubledCoordsHexGrid::HexesOnLine (unsigned int from, unsigned int to) const
{
    std::vector <unsigned int> result;
    result.push_back (from);
    auto distance = static_cast <unsigned int> (HeuristicDistance (from, to));

    std::pair <float, float> fromPosition = GetCellPosition (from);
    std::pair <float, float> toPosition = GetCellPosition (to);

    float deltaX = (toPosition.first - fromPosition.first) / distance;
    float deltaY = (toPosition.second - fromPosition.second) / distance;

    float x = fromPosition.first;
    float y = fromPosition.second;

    for (unsigned int iteration = 0; iteration < distance; ++iteration)
    {
        x += deltaX;
        y += deltaY;
        result.push_back (WorldPositionToCell (x, y));
    }

    return result;
}

std::vector <unsigned int>
DoubledCoordsHexGrid::HexesOnLine (unsigned int fromRow, unsigned int fromCol, unsigned int toRow,
        unsigned int toCol) const
{
    return HexesOnLine (EncodeCellPosition (fromRow, fromCol), EncodeCellPosition (toRow, toCol));
}

std::vector <unsigned int> DoubledCoordsHexGrid::Range (unsigned int center, unsigned int range) const
{
    std::unordered_set <unsigned int> blocked;
    std::queue <unsigned int> queue;

    unsigned int currentRange = 0;
    queue.push (center);
    blocked.insert (center);

    while (currentRange <= range && !queue.empty ())
    {
        unsigned int levelSize = queue.size ();
        for (unsigned int index = 0; index < levelSize; ++index)
        {
            unsigned int cell = queue.front ();
            queue.pop ();


            unsigned int row, col;
            DecodeCellPosition (cell, row, col);

            DoubledCoordsHexGridNeighborsIterator iterator (this, row, col, false);
            while (iterator.Valid ())
            {
                if (blocked.count (iterator.Get ().target) == 0)
                {
                    queue.push (iterator.Get ().target);
                    blocked.insert (cell);
                }
                iterator.Increment ();
            }
        }

        ++currentRange;
    }

    std::vector <unsigned int> result;
    result.reserve (blocked.size ());

    for (auto &cell : blocked)
    {
        result.push_back (cell);
    }
    return result;
}

std::vector <unsigned int>
DoubledCoordsHexGrid::Range (unsigned int centerRow, unsigned int centerCol, unsigned int range) const
{
    return Range (EncodeCellPosition (centerRow, centerCol), range);
}

DoubledCoordsHexGrid::Type DoubledCoordsHexGrid::GetType () const
{
    return type_;
}

unsigned int DoubledCoordsHexGrid::GetMaxRow () const
{
    return maxRow_;
}

unsigned int DoubledCoordsHexGrid::GetMaxCol () const
{
    return maxCol_;
}

float DoubledCoordsHexGrid::GetHexRadius () const
{
    return hexRadius_;
}

std::pair <float, float> DoubledCoordsHexGrid::GetCellPosition (unsigned int row, unsigned int col) const
{
    float x, y;
    float w = type_ == Type::DoubleWidth ? hexRadius_ * (float) std::sqrt (3) : hexRadius_ * 2.0f;
    float h = type_ == Type::DoubleHeight ? hexRadius_ * (float) std::sqrt (3) : hexRadius_ * 2.0f;

    if (type_ == Type::DoubleWidth)
    {
        x = w / 2.0f + w * col / 2.0f;
        y = hexRadius_ + h * row * 3.0f / 4.0f;
    }
    else
    {
        x = hexRadius_ + w * col * 3.0f / 4.0f;
        y = h / 2.0f + h * row / 2.0f;
    }

    return std::make_pair (x, y);
}

std::pair <float, float> DoubledCoordsHexGrid::GetCellPosition (unsigned int cell) const
{
    unsigned int row, col;
    DecodeCellPosition (cell, row, col);
    return GetCellPosition (row, col);
}

void DoubledCoordsHexGrid::WorldPositionToCell (float worldX, float worldY, unsigned int &row, unsigned int &col) const
{
    float w = type_ == Type::DoubleWidth ? hexRadius_ * (float) std::sqrt (3) : hexRadius_ * 2.0f;
    float h = type_ == Type::DoubleHeight ? hexRadius_ * (float) std::sqrt (3) : hexRadius_ * 2.0f;

    if (type_ == Type::DoubleWidth)
    {
        row = static_cast <unsigned int> (std::round ((worldY - hexRadius_) * 4.0f / (h * 3.0f)));
        if (row % 2 == 1)
        {
            worldX -= w / 2.0f;
        }

        col = static_cast <unsigned int> (std::round ((worldX - w / 2.0f) / w)) * 2;
        if (row % 2 == 1)
        {
            ++col;
        }
    }
    else
    {
        col = static_cast <unsigned int> (std::round ((worldX - hexRadius_) * 4.0f / (w * 3.0f)));
        if (col % 2 == 1)
        {
            worldY -= h / 2.0f;
        }

        row = static_cast <unsigned int> (std::round ((worldY - h / 2.0f) * 2.0f / h));

        if (col % 2 == 1)
        {
            ++row;
        }
    }
}

unsigned int DoubledCoordsHexGrid::WorldPositionToCell (float worldX, float worldY) const
{
    unsigned int row, col;
    WorldPositionToCell (worldX, worldY, row, col);
    return EncodeCellPosition (row, col);
}

void DoubledCoordsHexGrid::SetCell (unsigned int row, unsigned int col, float costModifier,
        SharedPointer <ReferenceCounted> metadata)
{
    SetCell (EncodeCellPosition (row, col), costModifier, metadata);
}

void DoubledCoordsHexGrid::SetCell (unsigned int cell, float costModifier, SharedPointer <ReferenceCounted> metadata)
{
    costModifiers_[cell] = costModifier;
    // unordered_map operates with objects in a "so dirty" way, so we must increment ref counter.
    metadata->AddReference ();
    metadata_[cell] = metadata;
}

float DoubledCoordsHexGrid::GetCellCostModifier (unsigned int row, unsigned int col) const
{
    return GetCellCostModifier (EncodeCellPosition (row, col));
}

float DoubledCoordsHexGrid::GetCellCostModifier (unsigned int cell) const
{
    try
    {
        return costModifiers_.at (cell);
    }
    catch (std::out_of_range &exception)
    {
        throw UniversalException <DoubledCoordsHexGrid::UnknownHash> (
                std::string (
                        "DoubledCoordsHexGrid: unknown hash exception while calling std methods. STD exception: ") +
                        exception.what ()
        );
    }
}

SharedPointer <ReferenceCounted> DoubledCoordsHexGrid::GetCellMetadata (unsigned int row, unsigned int col) const
{
    return GetCellMetadata (EncodeCellPosition (row, col));
}

SharedPointer <ReferenceCounted> DoubledCoordsHexGrid::GetCellMetadata (unsigned int cell) const
{
    try
    {
        return metadata_.at (cell);
    }
    catch (std::out_of_range &exception)
    {
        throw UniversalException <DoubledCoordsHexGrid::UnknownHash> (
                std::string (
                        "DoubledCoordsHexGrid: unknown hash exception while calling std methods. STD exception: ") +
                        exception.what ()
        );
    }
}

const std::unordered_map <unsigned int, float> &DoubledCoordsHexGrid::GetCostModifiers () const
{
    return costModifiers_;
}

const std::unordered_map <unsigned int, SharedPointer <ReferenceCounted>> &DoubledCoordsHexGrid::GetMetadata () const
{
    return metadata_;
}
