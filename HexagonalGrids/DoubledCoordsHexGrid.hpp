#pragma once

#include <GraphPathfinding/GraphAdapter.hpp>
#include <SharedPointer/SharedPointer.hpp>
#include <SharedPointer/ReferenceCounted.hpp>

#include <unordered_map>
#include <vector>
#include <functional>

class DoubledCoordsHexGrid : public GraphAdapter
{
public:
    typedef std::function <float (float from, float to)> CostCalculator;
    static float MedianCostCalculator (float from, float to);

    class DoubledCoordsHexGridNeighborsIterator : public SimpleIterator <VertexOutcomingConnection>
    {
    public:
        DoubledCoordsHexGridNeighborsIterator (const DoubledCoordsHexGrid *ownerGrid,
                unsigned int currentRow, unsigned int currentCol, bool ignoreImpassable);
        virtual ~DoubledCoordsHexGridNeighborsIterator () = default;

        virtual void Increment ();
        virtual VertexOutcomingConnection Get ();
        virtual bool Valid ();

        // Exceptions classes.
        class Invalid;

    private:
        const DoubledCoordsHexGrid *ownerGrid_;
        unsigned int moveIndex_;
        unsigned int currentRow_;
        unsigned int currentCol_;
        bool ignoreImpassable_;
    };

    enum class Type
    {
        DoubleWidth = 0,
        DoubleHeight
    };

    DoubledCoordsHexGrid (Type type, unsigned int maxRow, unsigned int maxCol, float hexRadius,
            const CostCalculator &costCalculator = MedianCostCalculator);
    virtual ~DoubledCoordsHexGrid () = default;

    virtual SimpleIterator <VertexOutcomingConnection> *GetOutcomingConnections (int vertex) const;
    SimpleIterator <VertexOutcomingConnection> *GetOutcomingConnections (unsigned int vertex,
            bool ignoreImpassable) const;
    virtual float HeuristicDistance (int beginVertex, int endVertex) const;

    unsigned int EncodeCellPosition (unsigned int row, unsigned int col) const;
    void DecodeCellPosition (unsigned int vertex, unsigned int &row, unsigned int &col) const;
    const std::vector <std::pair <int, int> > *GetAvailableMovesList () const;
    float GetCostBetween (unsigned int fromHash, unsigned int toHash) const;

    float GetRectWidth () const;
    float GetRectHeight () const;

    std::vector <unsigned int> HexesOnLine (unsigned int from, unsigned int to) const;
    std::vector <unsigned int> HexesOnLine (unsigned int fromRow, unsigned int fromCol,
            unsigned int toRow, unsigned int toCol) const;

    std::vector <unsigned int> Range (unsigned int center, unsigned int range) const;
    std::vector <unsigned int> Range (unsigned int centerRow, unsigned int centerCol, unsigned int range) const;

    Type GetType () const;
    unsigned int GetMaxRow () const;
    unsigned int GetMaxCol () const;
    float GetHexRadius () const;

    std::pair <float, float> GetCellPosition (unsigned int row, unsigned int col) const;
    std::pair <float, float> GetCellPosition (unsigned int cell) const;

    void WorldPositionToCell (float worldX, float worldY, unsigned int &row, unsigned int &col) const;
    unsigned int WorldPositionToCell (float worldX, float worldY) const;

    void SetCell (unsigned int row, unsigned int col, float costModifier, SharedPointer <ReferenceCounted> metadata);
    void SetCell (unsigned int cell, float costModifier, SharedPointer <ReferenceCounted> metadata);

    float GetCellCostModifier (unsigned int row, unsigned int col) const;
    float GetCellCostModifier (unsigned int cell) const;

    SharedPointer <ReferenceCounted> GetCellMetadata (unsigned int row, unsigned int col) const;
    SharedPointer <ReferenceCounted> GetCellMetadata (unsigned int cell) const;

    const std::unordered_map <unsigned int, float> &GetCostModifiers () const;
    const std::unordered_map <unsigned int, SharedPointer <ReferenceCounted>> &GetMetadata () const;

    // Exceptions classes.
    class UnknownHash;

private:
    static const std::vector <std::pair <int, int> > DoubleWidthAvailableMoves;
    static const std::vector <std::pair <int, int> > DoubleHeightAvailableMoves;

    Type type_;
    unsigned int maxRow_;
    unsigned int maxCol_;
    float hexRadius_;

    CostCalculator costCalculator_;
    std::unordered_map <unsigned int, float> costModifiers_;
    std::unordered_map <unsigned int, SharedPointer <ReferenceCounted> > metadata_;
};
