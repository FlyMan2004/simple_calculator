#pragma once
#include <utility>
#ifndef SIMPLE_CALC
#define SIMPLE_CALC

#include <type_traits>
#include <concepts>
#include <vector>
#include <memory>

#define fn  auto
#define let auto
#define STD ::std


namespace simple_calc
{

struct TreeNodeBase;

template <typename TreeNode>
concept TreeNodeTraits = requires (TreeNode tree_node) 
{
    requires STD::derived_from<TreeNode, TreeNodeBase>;
    typename TreeNode::Value;
    requires STD::is_object_v<typename TreeNode::Value> || STD::is_void_v<typename TreeNode::Value>;
};

struct TreeNodeBase {
protected:
    STD::vector<STD::shared_ptr<TreeNodeBase>> m_children{};

    TreeNodeBase() = default;
    constexpr TreeNodeBase(TreeNodeBase const&) = delete;
    constexpr TreeNodeBase& operator=(TreeNodeBase const&) = delete;

public:
    template <typename TreeNode>
    struct CreateFn {
        static constexpr fn operator()(...) -> void 
            requires (TreeNodeTraits<TreeNode> == false)
        = delete;

        template <typename... Args>
        static fn operator()(Args&&... args) -> STD::shared_ptr<TreeNode>
            requires TreeNodeTraits<TreeNode>
        {
            let node_ptr = STD::make_shared<TreeNode>(STD::forward<decltype(args)>(args)...);
            return node_ptr;
        }
    };

#ifdef Def_Wrapper
#undef Def_Wrapper
#endif /* defined(Def_Wrapper) */

#define Def_Wrapper(func) \
    template <TreeNodeTraits TreeNode> \
    [[gnu::always_inline]] \
    fn func(this TreeNode&& self) noexcept(noexcept(self.m_children.func())) -> auto \
    { return self.m_children.func(); }

    Def_Wrapper(begin)
    Def_Wrapper(end)
    Def_Wrapper(cbegin)
    Def_Wrapper(cend)
    Def_Wrapper(rbegin)
    Def_Wrapper(rend)
    Def_Wrapper(crbegin)
    Def_Wrapper(crend)

#undef Def_Wrapper

#define Def_Wrapper(interface, impl) \
    template <TreeNodeTraits TreeNode> \
    [[gnu::always_inline]] \
    fn interface(this TreeNode&& self) noexcept(noexcept(self.m_children.impl())) -> auto \
    { return self.m_children.impl(); }

    Def_Wrapper(Get_Number_Of_Children, size)
    Def_Wrapper(Get_Maximum_Of_Children, max_size)
    Def_Wrapper(Is_Leaf, empty)

#undef Def_Wrapper

#define Def_Wrapper(interface, impl) \
    template <TreeNodeTraits TreeNode> \
    [[gnu::always_inline]] \
    fn interface(this TreeNode&& self, auto&&... args) noexcept(noexcept(self.m_children.impl(STD::forward<decltype(args)>(args)...))) -> auto \
    { return self.m_children.impl(STD::forward<decltype(args)>(args)...); }

    Def_Wrapper(Remove_Children, erase)

#undef Def_Wrapper

    template <TreeNodeTraits TreeNode, typename... Args>
    fn Add_Child(this TreeNode& self, Args&&... args) -> void
    {
        let leaf = TreeNode::template Create(self.weak_from_this(), STD::forward<decltype(args)>(args)...);
        self.m_children.push_back(STD::move(leaf));
    }

};

template <typename Content>
struct TreeNode :
    public TreeNodeBase
{
    using Value = Content;
    using Interface = TreeNodeBase;
    using SharedThis = STD::enable_shared_from_this<TreeNode<Content>>;
    let static constexpr Create = typename Interface::template CreateFn<TreeNode<Value>>{};
    Value m_value;

    TreeNode();
    TreeNode(TreeNode&&) noexcept;
    TreeNode& operator=(TreeNode&&) noexcept;
    ~TreeNode() = default;
};

template <typename Content>
struct Tree
{
    STD::shared_ptr<TreeNode<Content>> m_root{TreeNode<Content>::Create()};
};

template struct TreeNode<int>;
template struct Tree<int>;

}

#undef fn
#undef let
#undef STD

#endif