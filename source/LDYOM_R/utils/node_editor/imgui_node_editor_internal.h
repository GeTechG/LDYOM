//------------------------------------------------------------------------------
// VERSION 0.9.1
//
// LICENSE
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// CREDITS
//   Written by Michal Cichon
//------------------------------------------------------------------------------
# ifndef __IMGUI_NODE_EDITOR_INTERNAL_H__
# define __IMGUI_NODE_EDITOR_INTERNAL_H__
# pragma once


//------------------------------------------------------------------------------
# ifndef IMGUI_DEFINE_MATH_OPERATORS
#     define IMGUI_DEFINE_MATH_OPERATORS
# endif
# include "imgui_node_editor.h"


//------------------------------------------------------------------------------
# include <imgui.h>
# include <imgui_internal.h>
# include "imgui_bezier_math.h"
# include "imgui_canvas.h"
# include "imgui_extra_math.h"

# include <string>
# include <vector>


//------------------------------------------------------------------------------
namespace ax {
	namespace NodeEditor {
		namespace Detail {
			//------------------------------------------------------------------------------
			namespace ed = Detail;


			//------------------------------------------------------------------------------
			using std::vector;
			using std::string;


			//------------------------------------------------------------------------------
			void Log(const char *fmt, ...);


			//------------------------------------------------------------------------------
			//inline ImRect ToRect(const ax::rectf& rect);
			//inline ImRect ToRect(const ax::rect& rect);
			inline ImRect ImGui_GetItemRect();
			inline ImVec2 ImGui_GetMouseClickPos(ImGuiMouseButton buttonIndex);


			//------------------------------------------------------------------------------
			// https://stackoverflow.com/a/36079786
# define DECLARE_HAS_MEMBER(__trait_name__, __member_name__)                         \
                                                                                     \
    template <typename __boost_has_member_T__>                                       \
    class __trait_name__                                                             \
    {                                                                                \
        using check_type = ::std::remove_const_t<__boost_has_member_T__>;            \
        struct no_type {char x[2];};                                                 \
        using  yes_type = char;                                                      \
                                                                                     \
        struct  base { void __member_name__() {}};                                   \
        struct mixin : public base, public check_type {};                            \
                                                                                     \
        template <void (base::*)()> struct aux {};                                   \
                                                                                     \
        template <typename U> static no_type  test(aux<&U::__member_name__>*);       \
        template <typename U> static yes_type test(...);                             \
                                                                                     \
        public:                                                                      \
                                                                                     \
        static constexpr bool value = (sizeof(yes_type) == sizeof(test<mixin>(0)));  \
    }

			DECLARE_HAS_MEMBER(HasFringeScale, _FringeScale);

# undef DECLARE_HAS_MEMBER

			struct FringeScaleRef {
				// Overload is present when ImDrawList does have _FringeScale member variable.
				template <typename T>
				static float& Get(std::enable_if_t<HasFringeScale<T>::value, T> *drawList) {
					return drawList->_FringeScale;
				}

				// Overload is present when ImDrawList does not have _FringeScale member variable.
				template <typename T>
				static float& Get(std::enable_if_t<!HasFringeScale<T>::value, T> *) {
					static float placeholder = 1.0f;
					return placeholder;
				}
			};

			static inline float& ImFringeScaleRef(ImDrawList *drawList) {
				return FringeScaleRef::Get<ImDrawList>(drawList);
			}

			struct FringeScaleScope {
				FringeScaleScope(float scale)
					: m_LastFringeScale(ImFringeScaleRef(ImGui::GetWindowDrawList())) {
					ImFringeScaleRef(ImGui::GetWindowDrawList()) = scale;
				}

				~FringeScaleScope() {
					ImFringeScaleRef(ImGui::GetWindowDrawList()) = m_LastFringeScale;
				}

			private:
				float m_LastFringeScale;
			};


			//------------------------------------------------------------------------------
			enum class ObjectType {
				None,
				Node,
				Link,
				Pin
			};

			using NodeEditor::PinKind;
			using NodeEditor::StyleColor;
			using NodeEditor::StyleVar;
			using NodeEditor::SaveReasonFlags;

			using NodeEditor::NodeId;
			using NodeEditor::PinId;
			using NodeEditor::LinkId;

			struct ObjectId final : Details::SafePointerType<ObjectId> {
				using Super = SafePointerType<ObjectId>;
				using Super::Super;

				ObjectId(): Super(Invalid), m_Type(ObjectType::None) {}
				ObjectId(PinId pinId): Super(pinId.AsPointer()), m_Type(ObjectType::Pin) {}
				ObjectId(NodeId nodeId): Super(nodeId.AsPointer()), m_Type(ObjectType::Node) {}
				ObjectId(LinkId linkId): Super(linkId.AsPointer()), m_Type(ObjectType::Link) {}

				explicit operator PinId() const { return AsPinId(); }
				explicit operator NodeId() const { return AsNodeId(); }
				explicit operator LinkId() const { return AsLinkId(); }

				PinId AsPinId() const {
					IM_ASSERT(IsPinId());
					return PinId(AsPointer());
				}

				NodeId AsNodeId() const {
					IM_ASSERT(IsNodeId());
					return NodeId(AsPointer());
				}

				LinkId AsLinkId() const {
					IM_ASSERT(IsLinkId());
					return LinkId(AsPointer());
				}

				bool IsPinId() const { return m_Type == ObjectType::Pin; }
				bool IsNodeId() const { return m_Type == ObjectType::Node; }
				bool IsLinkId() const { return m_Type == ObjectType::Link; }

				ObjectType Type() const { return m_Type; }

			private:
				ObjectType m_Type;
			};

			struct EditorContext;

			struct Node;
			struct Pin;
			struct Link;

			template <typename T, typename Id = typename T::IdType>
			struct ObjectWrapper {
				Id m_ID;
				T *m_Object;

				T* operator->() { return m_Object; }
				const T* operator->() const { return m_Object; }

				operator T*() { return m_Object; }
				operator const T*() const { return m_Object; }

				bool operator<(const ObjectWrapper &rhs) const {
					return m_ID.AsPointer() < rhs.m_ID.AsPointer();
				}
			};

			struct Object {
				enum DrawFlags {
					None = 0,
					Hovered = 1,
					Selected = 2,
					Highlighted = 4,
				};

				friend DrawFlags operator|(DrawFlags lhs, DrawFlags rhs) {
					return static_cast<DrawFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
				}

				friend DrawFlags operator&(DrawFlags lhs, DrawFlags rhs) {
					return static_cast<DrawFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
				}

				friend DrawFlags& operator|=(DrawFlags &lhs, DrawFlags rhs) {
					lhs = lhs | rhs;
					return lhs;
				}

				friend DrawFlags& operator&=(DrawFlags &lhs, DrawFlags rhs) {
					lhs = lhs & rhs;
					return lhs;
				}

				EditorContext *const Editor;

				bool m_IsLive;
				bool m_IsSelected;
				bool m_DeleteOnNewFrame;

				Object(EditorContext *editor)
					: Editor(editor)
					  , m_IsLive(true)
					  , m_IsSelected(false)
					  , m_DeleteOnNewFrame(false) { }

				virtual ~Object() = default;

				virtual ObjectId ID() = 0;

				bool IsVisible() const {
					if (!m_IsLive)
						return false;

					const auto bounds = GetBounds();

					return ImGui::IsRectVisible(bounds.Min, bounds.Max);
				}

				virtual void Reset() { m_IsLive = false; }

				virtual void Draw(ImDrawList *drawList, DrawFlags flags = None) = 0;

				virtual bool AcceptDrag() { return false; }
				virtual void UpdateDrag(const ImVec2 &offset) { IM_UNUSED(offset); }
				virtual bool EndDrag() { return false; }
				virtual ImVec2 DragStartLocation() { return GetBounds().Min; }

				virtual bool IsDraggable() {
					bool result = AcceptDrag();
					EndDrag();
					return result;
				}

				virtual bool IsSelectable() { return false; }

				virtual bool TestHit(const ImVec2 &point, float extraThickness = 0.0f) const {
					if (!m_IsLive)
						return false;

					auto bounds = GetBounds();
					if (extraThickness > 0)
						bounds.Expand(extraThickness);

					return bounds.Contains(point);
				}

				virtual bool TestHit(const ImRect &rect, bool allowIntersect = true) const {
					if (!m_IsLive)
						return false;

					const auto bounds = GetBounds();

					return !ImRect_IsEmpty(bounds) && (allowIntersect ? bounds.Overlaps(rect) : rect.Contains(bounds));
				}

				virtual ImRect GetBounds() const = 0;

				virtual Node* AsNode() { return nullptr; }
				virtual Pin* AsPin() { return nullptr; }
				virtual Link* AsLink() { return nullptr; }
			};

			struct Pin final : Object {
				using IdType = PinId;

				PinId m_ID;
				PinKind m_Kind;
				Node *m_Node;
				ImRect m_Bounds;
				ImRect m_Pivot;
				Pin *m_PreviousPin;
				ImU32 m_Color;
				ImU32 m_BorderColor;
				float m_BorderWidth;
				float m_Rounding;
				int m_Corners;
				ImVec2 m_Dir;
				float m_Strength;
				float m_Radius;
				float m_ArrowSize;
				float m_ArrowWidth;
				bool m_SnapLinkToDir;
				bool m_HasConnection;
				bool m_HadConnection;

				Pin(EditorContext *editor, PinId id, PinKind kind)
					: Object(editor)
					  , m_ID(id)
					  , m_Kind(kind)
					  , m_Node(nullptr)
					  , m_Bounds()
					  , m_PreviousPin(nullptr)
					  , m_Color(IM_COL32_WHITE)
					  , m_BorderColor(IM_COL32_BLACK)
					  , m_BorderWidth(0)
					  , m_Rounding(0)
					  , m_Corners(0)
					  , m_Dir(0, 0)
					  , m_Strength(0)
					  , m_Radius(0)
					  , m_ArrowSize(0)
					  , m_ArrowWidth(0)
					  , m_SnapLinkToDir(true)
					  , m_HasConnection(false)
					  , m_HadConnection(false) { }

				ObjectId ID() override { return m_ID; }

				void Reset() override {
					m_HadConnection = m_HasConnection && m_IsLive;
					m_HasConnection = false;

					Object::Reset();
				}

				void Draw(ImDrawList *drawList, DrawFlags flags = None) override;

				ImVec2 GetClosestPoint(const ImVec2 &p) const;
				ImLine GetClosestLine(const Pin *pin) const;

				ImRect GetBounds() const override { return m_Bounds; }

				Pin* AsPin() override { return this; }
			};

			enum class NodeType {
				Node,
				Group
			};

			enum class NodeRegion : uint8_t {
				None = 0x00,
				Top = 0x01,
				Bottom = 0x02,
				Left = 0x04,
				Right = 0x08,
				Center = 0x10,
				Header = 0x20,
				TopLeft = Top | Left,
				TopRight = Top | Right,
				BottomLeft = Bottom | Left,
				BottomRight = Bottom | Right,
			};

			inline NodeRegion operator |(NodeRegion lhs, NodeRegion rhs) {
				return static_cast<NodeRegion>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
			}

			inline NodeRegion operator &(NodeRegion lhs, NodeRegion rhs) {
				return static_cast<NodeRegion>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
			}


			struct Node final : Object {
				using IdType = NodeId;

				NodeId m_ID;
				NodeType m_Type;
				ImRect m_Bounds;
				float m_ZPosition;
				int m_Channel;
				Pin *m_LastPin;
				ImVec2 m_DragStart;

				ImU32 m_Color;
				ImU32 m_BorderColor;
				float m_BorderWidth;
				float m_Rounding;

				ImU32 m_GroupColor;
				ImU32 m_GroupBorderColor;
				float m_GroupBorderWidth;
				float m_GroupRounding;
				ImRect m_GroupBounds;

				bool m_HighlightConnectedLinks;

				bool m_RestoreState;
				bool m_CenterOnScreen;

				Node(EditorContext *editor, NodeId id)
					: Object(editor)
					  , m_ID(id)
					  , m_Type(NodeType::Node)
					  , m_Bounds()
					  , m_ZPosition(0.0f)
					  , m_Channel(0)
					  , m_LastPin(nullptr)
					  , m_DragStart()
					  , m_Color(IM_COL32_WHITE)
					  , m_BorderColor(IM_COL32_BLACK)
					  , m_BorderWidth(0)
					  , m_Rounding(0)
					  , m_GroupBounds()
					  , m_HighlightConnectedLinks(false)
					  , m_RestoreState(false)
					  , m_CenterOnScreen(false) { }

				ObjectId ID() override { return m_ID; }

				bool AcceptDrag() override;
				void UpdateDrag(const ImVec2 &offset) override;
				bool EndDrag() override; // return true, when changed
				ImVec2 DragStartLocation() override { return m_DragStart; }

				bool IsSelectable() override { return true; }

				void Draw(ImDrawList *drawList, DrawFlags flags = None) override;
				void DrawBorder(ImDrawList *drawList, ImU32 color, float thickness = 1.0f, float offset = 0.0f);

				void GetGroupedNodes(std::vector<Node*> &result, bool append = false);

				void CenterOnScreenInNextFrame() { m_CenterOnScreen = true; }

				ImRect GetRegionBounds(NodeRegion region) const;
				NodeRegion GetRegion(const ImVec2 &point) const;

				ImRect GetBounds() const override { return m_Bounds; }

				Node* AsNode() override { return this; }
			};

			struct Link final : Object {
				using IdType = LinkId;

				LinkId m_ID;
				Pin *m_StartPin;
				Pin *m_EndPin;
				ImU32 m_Color;
				ImU32 m_HighlightColor;
				float m_Thickness;
				ImVec2 m_Start;
				ImVec2 m_End;

				Link(EditorContext *editor, LinkId id)
					: Object(editor)
					  , m_ID(id)
					  , m_StartPin(nullptr)
					  , m_EndPin(nullptr)
					  , m_Color(IM_COL32_WHITE)
					  , m_Thickness(1.0f) { }

				ObjectId ID() override { return m_ID; }

				bool IsSelectable() override { return true; }

				void Draw(ImDrawList *drawList, DrawFlags flags = None) override;
				void Draw(ImDrawList *drawList, ImU32 color, float extraThickness = 0.0f) const;

				void UpdateEndpoints();

				ImCubicBezierPoints GetCurve() const;

				bool TestHit(const ImVec2 &point, float extraThickness = 0.0f) const override;
				bool TestHit(const ImRect &rect, bool allowIntersect = true) const override;

				ImRect GetBounds() const override;

				Link* AsLink() override { return this; }
			};

			struct NodeSettings {
				NodeId m_ID;
				ImVec2 m_Location;
				ImVec2 m_Size;
				ImVec2 m_GroupSize;
				bool m_WasUsed;

				bool m_Saved;
				bool m_IsDirty;
				SaveReasonFlags m_DirtyReason;

				NodeSettings(NodeId id)
					: m_ID(id)
					  , m_Location(0, 0)
					  , m_Size(0, 0)
					  , m_GroupSize(0, 0)
					  , m_WasUsed(false)
					  , m_Saved(false)
					  , m_IsDirty(false)
					  , m_DirtyReason(SaveReasonFlags::None) { }

				void ClearDirty();
				void MakeDirty(SaveReasonFlags reason);
			};

			struct Settings {
				bool m_IsDirty;
				SaveReasonFlags m_DirtyReason;

				vector<NodeSettings> m_Nodes;
				vector<ObjectId> m_Selection;
				ImVec2 m_ViewScroll;
				float m_ViewZoom;
				ImRect m_VisibleRect;

				Settings()
					: m_IsDirty(false)
					  , m_DirtyReason(SaveReasonFlags::None)
					  , m_ViewScroll(0, 0)
					  , m_ViewZoom(1.0f)
					  , m_VisibleRect() { }

				NodeSettings* AddNode(NodeId id);
				NodeSettings* FindNode(NodeId id);
				void RemoveNode(NodeId id);

				void ClearDirty(Node *node = nullptr);
				void MakeDirty(SaveReasonFlags reason, Node *node = nullptr);
			};

			struct Control {
				Object *HotObject;
				Object *ActiveObject;
				Object *ClickedObject;
				Object *DoubleClickedObject;
				Node *HotNode;
				Node *ActiveNode;
				Node *ClickedNode;
				Node *DoubleClickedNode;
				Pin *HotPin;
				Pin *ActivePin;
				Pin *ClickedPin;
				Pin *DoubleClickedPin;
				Link *HotLink;
				Link *ActiveLink;
				Link *ClickedLink;
				Link *DoubleClickedLink;
				bool BackgroundHot;
				bool BackgroundActive;
				int BackgroundClickButtonIndex;
				int BackgroundDoubleClickButtonIndex;

				Control()
					: Control(nullptr, nullptr, nullptr, nullptr, false, false, -1, -1) { }

				Control(Object *hotObject, Object *activeObject, Object *clickedObject, Object *doubleClickedObject,
				        bool backgroundHot, bool backgroundActive, int backgroundClickButtonIndex,
				        int backgroundDoubleClickButtonIndex)
					: HotObject(hotObject)
					  , ActiveObject(activeObject)
					  , ClickedObject(clickedObject)
					  , DoubleClickedObject(doubleClickedObject)
					  , HotNode(nullptr)
					  , ActiveNode(nullptr)
					  , ClickedNode(nullptr)
					  , DoubleClickedNode(nullptr)
					  , HotPin(nullptr)
					  , ActivePin(nullptr)
					  , ClickedPin(nullptr)
					  , DoubleClickedPin(nullptr)
					  , HotLink(nullptr)
					  , ActiveLink(nullptr)
					  , ClickedLink(nullptr)
					  , DoubleClickedLink(nullptr)
					  , BackgroundHot(backgroundHot)
					  , BackgroundActive(backgroundActive)
					  , BackgroundClickButtonIndex(backgroundClickButtonIndex)
					  , BackgroundDoubleClickButtonIndex(backgroundDoubleClickButtonIndex) {
					if (hotObject) {
						HotNode = hotObject->AsNode();
						HotPin = hotObject->AsPin();
						HotLink = hotObject->AsLink();

						if (HotPin)
							HotNode = HotPin->m_Node;
					}

					if (activeObject) {
						ActiveNode = activeObject->AsNode();
						ActivePin = activeObject->AsPin();
						ActiveLink = activeObject->AsLink();
					}

					if (clickedObject) {
						ClickedNode = clickedObject->AsNode();
						ClickedPin = clickedObject->AsPin();
						ClickedLink = clickedObject->AsLink();
					}

					if (doubleClickedObject) {
						DoubleClickedNode = doubleClickedObject->AsNode();
						DoubleClickedPin = doubleClickedObject->AsPin();
						DoubleClickedLink = doubleClickedObject->AsLink();
					}
				}
			};

			struct NavigateAction;
			struct SizeAction;
			struct DragAction;
			struct SelectAction;
			struct CreateItemAction;
			struct DeleteItemsAction;
			struct ContextMenuAction;
			struct ShortcutAction;

			struct AnimationController;
			struct FlowAnimationController;

			struct Animation {
				enum State {
					Playing,
					Stopped
				};

				EditorContext *Editor;
				State m_State;
				float m_Time;
				float m_Duration;

				Animation(EditorContext *editor);
				virtual ~Animation();

				void Play(float duration);
				void Stop();
				void Finish();
				void Update();

				bool IsPlaying() const { return m_State == Playing; }

				float GetProgress() const { return m_Time / m_Duration; }

			protected:
				virtual void OnPlay() {}
				virtual void OnFinish() {}
				virtual void OnStop() {}

				virtual void OnUpdate(float progress) { IM_UNUSED(progress); }
			};

			struct NavigateAnimation final : Animation {
				NavigateAction &Action;
				ImRect m_Start;
				ImRect m_Target;

				NavigateAnimation(EditorContext *editor, NavigateAction &scrollAction);

				void NavigateTo(const ImRect &target, float duration);

			private:
				void OnUpdate(float progress) override;
				void OnStop() override;
				void OnFinish() override;
			};

			struct FlowAnimation final : Animation {
				FlowAnimationController *Controller;
				Link *m_Link;
				float m_Speed;
				float m_MarkerDistance;
				float m_Offset;

				FlowAnimation(FlowAnimationController *controller);

				void Flow(Link *link, float markerDistance, float speed, float duration);

				void Draw(ImDrawList *drawList);

			private:
				struct CurvePoint {
					float Distance;
					ImVec2 Point;
				};

				ImVec2 m_LastStart;
				ImVec2 m_LastEnd;
				float m_PathLength;
				vector<CurvePoint> m_Path;

				bool IsLinkValid() const;
				bool IsPathValid() const;
				void UpdatePath();
				void ClearPath();

				ImVec2 SamplePath(float distance) const;

				void OnUpdate(float progress) override;
				void OnStop() override;
			};

			struct AnimationController {
				EditorContext *Editor;

				AnimationController(EditorContext *editor)
					: Editor(editor) { }

				virtual ~AnimationController() { }

				virtual void Draw(ImDrawList *drawList) {
					IM_UNUSED(drawList);
				}
			};

			struct FlowAnimationController final : AnimationController {
				FlowAnimationController(EditorContext *editor);
				~FlowAnimationController() override;

				void Flow(Link *link, FlowDirection direction = FlowDirection::Forward);

				void Draw(ImDrawList *drawList) override;

				void Release(FlowAnimation *animation);

			private:
				FlowAnimation* GetOrCreate(Link *link);

				vector<FlowAnimation*> m_Animations;
				vector<FlowAnimation*> m_FreePool;
			};

			struct EditorAction {
				enum AcceptResult { False, True, Possible };

				EditorAction(EditorContext *editor)
					: Editor(editor) { }

				virtual ~EditorAction() {}

				virtual const char* GetName() const = 0;

				virtual AcceptResult Accept(const Control &control) = 0;
				virtual bool Process(const Control &control) = 0;
				virtual void Reject() {} // celled when Accept return 'Possible' and was rejected

				virtual ImGuiMouseCursor GetCursor() { return ImGuiMouseCursor_Arrow; }

				virtual bool IsDragging() { return false; }

				virtual void ShowMetrics() {}

				virtual NavigateAction* AsNavigate() { return nullptr; }
				virtual SizeAction* AsSize() { return nullptr; }
				virtual DragAction* AsDrag() { return nullptr; }
				virtual SelectAction* AsSelect() { return nullptr; }
				virtual CreateItemAction* AsCreateItem() { return nullptr; }
				virtual DeleteItemsAction* AsDeleteItems() { return nullptr; }
				virtual ContextMenuAction* AsContextMenu() { return nullptr; }
				virtual ShortcutAction* AsCutCopyPaste() { return nullptr; }

				EditorContext *Editor;
			};

			struct NavigateAction final : EditorAction {
				enum class ZoomMode {
					None,
					Exact,
					WithMargin
				};

				enum class NavigationReason {
					Unknown,
					MouseZoom,
					Selection,
					Object,
					Content,
					Edge
				};

				bool m_IsActive;
				float m_Zoom;
				ImRect m_VisibleRect;
				ImVec2 m_Scroll;
				ImVec2 m_ScrollStart;
				ImVec2 m_ScrollDelta;

				NavigateAction(EditorContext *editor, ImGuiEx::Canvas &canvas);

				const char* GetName() const override { return "Navigate"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				void ShowMetrics() override;

				NavigateAction* AsNavigate() override { return this; }

				void NavigateTo(const ImRect &bounds, ZoomMode zoomMode, float duration = -1.0f,
				                NavigationReason reason = NavigationReason::Unknown);
				void StopNavigation();
				void FinishNavigation();

				bool MoveOverEdge(const ImVec2 &canvasSize);
				void StopMoveOverEdge();
				bool IsMovingOverEdge() const { return m_MovingOverEdge; }
				ImVec2 GetMoveScreenOffset() const { return m_MoveScreenOffset; }

				void SetWindow(ImVec2 position, ImVec2 size);
				ImVec2 GetWindowScreenPos() const { return m_WindowScreenPos; };
				ImVec2 GetWindowScreenSize() const { return m_WindowScreenSize; };

				ImGuiEx::CanvasView GetView() const;
				ImVec2 GetViewOrigin() const;
				float GetViewScale() const;

				void SetViewRect(const ImRect &rect);
				ImRect GetViewRect() const;

			private:
				ImGuiEx::Canvas &m_Canvas;
				ImVec2 m_WindowScreenPos;
				ImVec2 m_WindowScreenSize;

				NavigateAnimation m_Animation;
				NavigationReason m_Reason;
				uint64_t m_LastSelectionId;
				Object *m_LastObject;
				bool m_MovingOverEdge;
				ImVec2 m_MoveScreenOffset;

				const float *m_ZoomLevels;
				int m_ZoomLevelCount;

				bool HandleZoom(const Control &control);

				void NavigateTo(const ImRect &target, float duration = -1.0f,
				                NavigationReason reason = NavigationReason::Unknown);

				float GetNextZoom(float steps);
				float MatchSmoothZoom(float steps);
				float MatchZoom(int steps, float fallbackZoom);
				int MatchZoomIndex(int direction);

				static const float s_DefaultZoomLevels[];
				static const int s_DefaultZoomLevelCount;
			};

			struct SizeAction final : EditorAction {
				bool m_IsActive;
				bool m_Clean;
				Node *m_SizedNode;

				SizeAction(EditorContext *editor);

				const char* GetName() const override { return "Size"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				ImGuiMouseCursor GetCursor() override { return m_Cursor; }

				void ShowMetrics() override;

				SizeAction* AsSize() override { return this; }

				bool IsDragging() override { return m_IsActive; }

				const ImRect& GetStartGroupBounds() const { return m_StartGroupBounds; }

			private:
				NodeRegion GetRegion(Node *node);
				ImGuiMouseCursor ChooseCursor(NodeRegion region);

				ImRect m_StartBounds;
				ImRect m_StartGroupBounds;
				ImVec2 m_LastSize;
				ImVec2 m_MinimumSize;
				ImVec2 m_LastDragOffset;
				NodeRegion m_Pivot;
				ImGuiMouseCursor m_Cursor;
			};

			struct DragAction final : EditorAction {
				bool m_IsActive;
				bool m_Clear;
				Object *m_DraggedObject;
				vector<Object*> m_Objects;

				DragAction(EditorContext *editor);

				const char* GetName() const override { return "Drag"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				ImGuiMouseCursor GetCursor() override { return ImGuiMouseCursor_ResizeAll; }

				bool IsDragging() override { return m_IsActive; }

				void ShowMetrics() override;

				DragAction* AsDrag() override { return this; }
			};

			struct SelectAction final : EditorAction {
				bool m_IsActive;

				bool m_SelectGroups;
				bool m_SelectLinkMode;
				bool m_CommitSelection;
				ImVec2 m_StartPoint;
				ImVec2 m_EndPoint;
				vector<Object*> m_CandidateObjects;
				vector<Object*> m_SelectedObjectsAtStart;

				Animation m_Animation;

				SelectAction(EditorContext *editor);

				const char* GetName() const override { return "Select"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				void ShowMetrics() override;

				bool IsDragging() override { return m_IsActive; }

				SelectAction* AsSelect() override { return this; }

				void Draw(ImDrawList *drawList);
			};

			struct ContextMenuAction final : EditorAction {
				enum Menu { None, Node, Pin, Link, Background };

				Menu m_CandidateMenu;
				Menu m_CurrentMenu;
				ObjectId m_ContextId;

				ContextMenuAction(EditorContext *editor);

				const char* GetName() const override { return "Context Menu"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;
				void Reject() override;

				void ShowMetrics() override;

				ContextMenuAction* AsContextMenu() override { return this; }

				bool ShowNodeContextMenu(NodeId *nodeId);
				bool ShowPinContextMenu(PinId *pinId);
				bool ShowLinkContextMenu(LinkId *linkId);
				bool ShowBackgroundContextMenu();
			};

			struct ShortcutAction final : EditorAction {
				enum Action { None, Cut, Copy, Paste, Duplicate, CreateNode };

				bool m_IsActive;
				bool m_InAction;
				Action m_CurrentAction;
				vector<Object*> m_Context;

				ShortcutAction(EditorContext *editor);

				const char* GetName() const override { return "Shortcut"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;
				void Reject() override;

				void ShowMetrics() override;

				ShortcutAction* AsCutCopyPaste() override { return this; }

				bool Begin();
				void End();

				bool AcceptCut();
				bool AcceptCopy();
				bool AcceptPaste();
				bool AcceptDuplicate();
				bool AcceptCreateNode();
			};

			struct CreateItemAction final : EditorAction {
				enum Stage {
					None,
					Possible,
					Create
				};

				enum Action {
					Unknown,
					UserReject,
					UserAccept
				};

				enum Type {
					NoItem,
					Node,
					Link
				};

				enum Result {
					True,
					False,
					Indeterminate
				};

				bool m_InActive;
				Stage m_NextStage;

				Stage m_CurrentStage;
				Type m_ItemType;
				Action m_UserAction;
				ImU32 m_LinkColor;
				float m_LinkThickness;
				Pin *m_LinkStart;
				Pin *m_LinkEnd;

				bool m_IsActive;
				Pin *m_DraggedPin;

				int m_LastChannel = -1;


				CreateItemAction(EditorContext *editor);

				const char* GetName() const override { return "Create Item"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				ImGuiMouseCursor GetCursor() override { return ImGuiMouseCursor_Arrow; }

				void ShowMetrics() override;

				bool IsDragging() override { return m_IsActive; }

				CreateItemAction* AsCreateItem() override { return this; }

				void SetStyle(ImU32 color, float thickness);

				bool Begin();
				void End();

				Result RejectItem();
				Result AcceptItem();

				Result QueryLink(PinId *startId, PinId *endId);
				Result QueryNode(PinId *pinId);

			private:
				bool m_IsInGlobalSpace;

				void DragStart(Pin *startPin);
				void DragEnd();
				void DropPin(Pin *endPin);
				void DropNode();
				void DropNothing();
			};

			struct DeleteItemsAction final : EditorAction {
				bool m_IsActive;
				bool m_InInteraction;

				DeleteItemsAction(EditorContext *editor);

				const char* GetName() const override { return "Delete Items"; }

				AcceptResult Accept(const Control &control) override;
				bool Process(const Control &control) override;

				void ShowMetrics() override;

				DeleteItemsAction* AsDeleteItems() override { return this; }

				bool Add(Object *object);

				bool Begin();
				void End();

				bool QueryLink(LinkId *linkId, PinId *startId = nullptr, PinId *endId = nullptr);
				bool QueryNode(NodeId *nodeId);

				bool AcceptItem(bool deleteDependencies);
				void RejectItem();

			private:
				enum IteratorType { Unknown, Link, Node };

				enum UserAction { Undetermined, Accepted, Rejected };

				void DeleteDeadLinks(NodeId nodeId);
				void DeleteDeadPins(NodeId nodeId);

				bool QueryItem(ObjectId *itemId, IteratorType itemType);
				void RemoveItem(bool deleteDependencies);
				Object* DropCurrentItem();

				vector<Object*> m_ManuallyDeletedObjects;

				IteratorType m_CurrentItemType;
				UserAction m_UserAction;
				vector<Object*> m_CandidateObjects;
				int m_CandidateItemIndex;
			};

			struct NodeBuilder {
				EditorContext *const Editor;

				Node *m_CurrentNode;
				Pin *m_CurrentPin;

				ImRect m_NodeRect;

				ImRect m_PivotRect;
				ImVec2 m_PivotAlignment;
				ImVec2 m_PivotSize;
				ImVec2 m_PivotScale;
				bool m_ResolvePinRect;
				bool m_ResolvePivot;

				ImRect m_GroupBounds;
				bool m_IsGroup;

				ImDrawListSplitter m_Splitter;
				ImDrawListSplitter m_PinSplitter;

				NodeBuilder(EditorContext *editor);
				~NodeBuilder();

				void Begin(NodeId nodeId);
				void End();

				void BeginPin(PinId pinId, PinKind kind);
				void EndPin();

				void PinRect(const ImVec2 &a, const ImVec2 &b);
				void PinPivotRect(const ImVec2 &a, const ImVec2 &b);
				void PinPivotSize(const ImVec2 &size);
				void PinPivotScale(const ImVec2 &scale);
				void PinPivotAlignment(const ImVec2 &alignment);

				void Group(const ImVec2 &size);

				ImDrawList* GetUserBackgroundDrawList() const;
				ImDrawList* GetUserBackgroundDrawList(Node *node) const;
			};

			struct HintBuilder {
				EditorContext *const Editor;
				bool m_IsActive;
				Node *m_CurrentNode;
				float m_LastFringe = 1.0f;
				int m_LastChannel = 0;

				HintBuilder(EditorContext *editor);

				bool Begin(NodeId nodeId);
				void End();

				ImVec2 GetGroupMin();
				ImVec2 GetGroupMax();

				ImDrawList* GetForegroundDrawList();
				ImDrawList* GetBackgroundDrawList();
			};

			struct Style : NodeEditor::Style {
				void PushColor(StyleColor colorIndex, const ImVec4 &color);
				void PopColor(int count = 1);

				void PushVar(StyleVar varIndex, float value);
				void PushVar(StyleVar varIndex, const ImVec2 &value);
				void PushVar(StyleVar varIndex, const ImVec4 &value);
				void PopVar(int count = 1);

				const char* GetColorName(StyleColor colorIndex) const;

			private:
				struct ColorModifier {
					StyleColor Index;
					ImVec4 Value;
				};

				struct VarModifier {
					StyleVar Index;
					ImVec4 Value;
				};

				float* GetVarFloatAddr(StyleVar idx);
				ImVec2* GetVarVec2Addr(StyleVar idx);
				ImVec4* GetVarVec4Addr(StyleVar idx);

				vector<ColorModifier> m_ColorStack;
				vector<VarModifier> m_VarStack;
			};

			struct Config : NodeEditor::Config {
				Config(const NodeEditor::Config *config);

				std::string Load();
				std::string LoadNode(NodeId nodeId);

				void BeginSave();
				bool Save(const std::string &data, SaveReasonFlags flags);
				bool SaveNode(NodeId nodeId, const std::string &data, SaveReasonFlags flags);
				void EndSave();
			};

			enum class SuspendFlags : uint8_t {
				None = 0,
				KeepSplitter = 1
			};

			inline SuspendFlags operator |(SuspendFlags lhs, SuspendFlags rhs) {
				return static_cast<SuspendFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
			}

			inline SuspendFlags operator &(SuspendFlags lhs, SuspendFlags rhs) {
				return static_cast<SuspendFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
			}


			struct EditorContext {
				EditorContext(const NodeEditor::Config *config = nullptr);
				~EditorContext();

				const Config& GetConfig() const { return m_Config; }

				Style& GetStyle() { return m_Style; }

				void Begin(const char *id, const ImVec2 &size = ImVec2(0, 0));
				void End();

				bool DoLink(LinkId id, PinId startPinId, PinId endPinId, ImU32 color, float thickness);


				NodeBuilder& GetNodeBuilder() { return m_NodeBuilder; }
				HintBuilder& GetHintBuilder() { return m_HintBuilder; }

				EditorAction* GetCurrentAction() { return m_CurrentAction; }

				CreateItemAction& GetItemCreator() { return m_CreateItemAction; }
				DeleteItemsAction& GetItemDeleter() { return m_DeleteItemsAction; }
				ContextMenuAction& GetContextMenu() { return m_ContextMenuAction; }
				ShortcutAction& GetShortcut() { return m_ShortcutAction; }

				const ImGuiEx::CanvasView& GetView() const { return m_Canvas.View(); }
				const ImRect& GetViewRect() const { return m_Canvas.ViewRect(); }
				const ImRect& GetRect() const { return m_Canvas.Rect(); }

				void SetNodePosition(NodeId nodeId, const ImVec2 &screenPosition);
				void SetGroupSize(NodeId nodeId, const ImVec2 &size);
				ImVec2 GetNodePosition(NodeId nodeId);
				ImVec2 GetNodeSize(NodeId nodeId);

				void SetNodeZPosition(NodeId nodeId, float z);
				float GetNodeZPosition(NodeId nodeId);

				void MarkNodeToRestoreState(Node *node);
				void UpdateNodeState(Node *node);

				void RemoveSettings(Object *object);

				void ClearSelection();
				void SelectObject(Object *object);
				void DeselectObject(Object *object);
				void SetSelectedObject(Object *object);
				void ToggleObjectSelection(Object *object);
				bool IsSelected(Object *object);
				const vector<Object*>& GetSelectedObjects();
				bool IsAnyNodeSelected();
				bool IsAnyLinkSelected();
				bool HasSelectionChanged();
				uint64_t GetSelectionId() const { return m_SelectionId; }

				Node* FindNodeAt(const ImVec2 &p);
				void FindNodesInRect(const ImRect &r, vector<Node*> &result, bool append = false,
				                     bool includeIntersecting = true);
				void FindLinksInRect(const ImRect &r, vector<Link*> &result, bool append = false);

				bool HasAnyLinks(NodeId nodeId) const;
				bool HasAnyLinks(PinId pinId) const;

				int BreakLinks(NodeId nodeId);
				int BreakLinks(PinId pinId);

				void FindLinksForNode(NodeId nodeId, vector<Link*> &result, bool add = false);

				bool PinHadAnyLinks(PinId pinId);

				ImVec2 ToCanvas(const ImVec2 &point) const { return m_Canvas.ToLocal(point); }
				ImVec2 ToScreen(const ImVec2 &point) const { return m_Canvas.FromLocal(point); }

				void NotifyLinkDeleted(Link *link);

				void Suspend(SuspendFlags flags = SuspendFlags::None);
				void Resume(SuspendFlags flags = SuspendFlags::None);
				bool IsSuspended();

				bool IsFocused();
				bool IsHovered() const;
				bool IsHoveredWithoutOverlapp() const;
				bool CanAcceptUserInput() const;

				void MakeDirty(SaveReasonFlags reason);
				void MakeDirty(SaveReasonFlags reason, Node *node);

				int CountLiveNodes() const;
				int CountLivePins() const;
				int CountLiveLinks() const;

				Pin* CreatePin(PinId id, PinKind kind);
				Node* CreateNode(NodeId id);
				Link* CreateLink(LinkId id);

				Node* FindNode(NodeId id);
				Pin* FindPin(PinId id);
				Link* FindLink(LinkId id);
				Object* FindObject(ObjectId id);

				Node* GetNode(NodeId id);
				Pin* GetPin(PinId id, PinKind kind);
				Link* GetLink(LinkId id);

				Link* FindLinkAt(const ImVec2 &p);

				template <typename T>
				ImRect GetBounds(const std::vector<T*> &objects) {
					ImRect bounds(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (auto object : objects)
						if (object->m_IsLive)
							bounds.Add(object->GetBounds());

					if (ImRect_IsEmpty(bounds))
						bounds = ImRect();

					return bounds;
				}

				template <typename T>
				ImRect GetBounds(const std::vector<ObjectWrapper<T>> &objects) {
					ImRect bounds(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (auto object : objects)
						if (object.m_Object->m_IsLive)
							bounds.Add(object.m_Object->GetBounds());

					if (ImRect_IsEmpty(bounds))
						bounds = ImRect();

					return bounds;
				}

				ImRect GetSelectionBounds() { return GetBounds(m_SelectedObjects); }
				ImRect GetContentBounds() { return GetBounds(m_Nodes); }

				ImU32 GetColor(StyleColor colorIndex) const;
				ImU32 GetColor(StyleColor colorIndex, float alpha) const;

				int GetNodeIds(NodeId *nodes, int size) const;

				void NavigateTo(const ImRect &bounds, bool zoomIn = false, float duration = -1) {
					auto zoomMode = zoomIn ? NavigateAction::ZoomMode::WithMargin : NavigateAction::ZoomMode::None;
					m_NavigateAction.NavigateTo(bounds, zoomMode, duration);
				}

				void RegisterAnimation(Animation *animation);
				void UnregisterAnimation(Animation *animation);

				void Flow(Link *link, FlowDirection direction);

				void SetUserContext(bool globalSpace = false);

				void EnableShortcuts(bool enable);
				bool AreShortcutsEnabled();

				NodeId GetHoveredNode() const { return m_HoveredNode; }
				PinId GetHoveredPin() const { return m_HoveredPin; }
				LinkId GetHoveredLink() const { return m_HoveredLink; }
				NodeId GetDoubleClickedNode() const { return m_DoubleClickedNode; }
				PinId GetDoubleClickedPin() const { return m_DoubleClickedPin; }
				LinkId GetDoubleClickedLink() const { return m_DoubleClickedLink; }
				bool IsBackgroundClicked() const { return m_BackgroundClickButtonIndex >= 0; }
				bool IsBackgroundDoubleClicked() const { return m_BackgroundDoubleClickButtonIndex >= 0; }
				ImGuiMouseButton GetBackgroundClickButtonIndex() const { return m_BackgroundClickButtonIndex; }

				ImGuiMouseButton GetBackgroundDoubleClickButtonIndex() const {
					return m_BackgroundDoubleClickButtonIndex;
				}

				float AlignPointToGrid(float p) const {
					if (!ImGui::GetIO().KeyAlt)
						return p - ImFmod(p, 16.0f);
					return p;
				}

				ImVec2 AlignPointToGrid(const ImVec2 &p) const {
					return ImVec2(AlignPointToGrid(p.x), AlignPointToGrid(p.y));
				}

				ImDrawList* GetDrawList() { return m_DrawList; }

			private:
				void LoadSettings();
				void SaveSettings();

				Control BuildControl(bool allowOffscreen);

				void ShowMetrics(const Control &control);

				void UpdateAnimations();

				Config m_Config;

				ImGuiID m_EditorActiveId;
				bool m_IsFirstFrame;
				bool m_IsFocused;
				bool m_IsHovered;
				bool m_IsHoveredWithoutOverlapp;

				bool m_ShortcutsEnabled;

				Style m_Style;

				vector<ObjectWrapper<Node>> m_Nodes;
				vector<ObjectWrapper<Pin>> m_Pins;
				vector<ObjectWrapper<Link>> m_Links;

				vector<Object*> m_SelectedObjects;

				vector<Object*> m_LastSelectedObjects;
				uint64_t m_SelectionId;

				Link *m_LastActiveLink;

				vector<Animation*> m_LiveAnimations;
				vector<Animation*> m_LastLiveAnimations;

				ImGuiEx::Canvas m_Canvas;
				bool m_IsCanvasVisible;

				NodeBuilder m_NodeBuilder;
				HintBuilder m_HintBuilder;

				EditorAction *m_CurrentAction;
				NavigateAction m_NavigateAction;
				SizeAction m_SizeAction;
				DragAction m_DragAction;
				SelectAction m_SelectAction;
				ContextMenuAction m_ContextMenuAction;
				ShortcutAction m_ShortcutAction;
				CreateItemAction m_CreateItemAction;
				DeleteItemsAction m_DeleteItemsAction;

				vector<AnimationController*> m_AnimationControllers;
				FlowAnimationController m_FlowAnimationController;

				NodeId m_HoveredNode;
				PinId m_HoveredPin;
				LinkId m_HoveredLink;
				NodeId m_DoubleClickedNode;
				PinId m_DoubleClickedPin;
				LinkId m_DoubleClickedLink;
				int m_BackgroundClickButtonIndex;
				int m_BackgroundDoubleClickButtonIndex;

				bool m_IsInitialized;
				Settings m_Settings;

				ImDrawList *m_DrawList;
				int m_ExternalChannel;
				ImDrawListSplitter m_Splitter;
			};


			//------------------------------------------------------------------------------
		} // namespace Detail
	} // namespace Editor
} // namespace ax


//------------------------------------------------------------------------------
# include "imgui_node_editor_internal.inl"


//------------------------------------------------------------------------------
# endif // __IMGUI_NODE_EDITOR_INTERNAL_H__