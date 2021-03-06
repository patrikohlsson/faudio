(in-package :faudio)
(define-foreign-type map-type () () (:actual-type :pointer))
(define-parse-method map () (make-instance 'map-type))
(defclass map () ((map-ptr :initarg :map-ptr)))
(defmethod translate-to-foreign (x (type map-type)) (slot-value x 'map-ptr))
(defmethod translate-from-foreign (x (type map-type)) (make-instance 'map :map-ptr x))
(defctype map-key ptr)
(defcfun (map-empty "fa_map_empty") map)
(defcfun (map-copy "fa_map_copy") map (a map))
(defcfun (map-destroy "fa_map_destroy") :void (a map))
(defcfun (map-size "fa_map_size") :int (a map))
(defcfun (map-is-empty "fa_map_is_empty") :boolean (a map))
(defcfun (map-is-single "fa_map_is_single") :boolean (a map))
(defcfun (map-add "fa_map_add") map (a map-key) (b ptr) (c map))
(defcfun (map-set "fa_map_set") map (a map-key) (b ptr) (c map))
(defcfun (map-remove "fa_map_remove") map (a map-key) (b map))
(defcfun (map-dadd "fa_map_dadd") map (a map-key) (b ptr) (c map))
(defcfun (map-dset "fa_map_dset") map (a map-key) (b ptr) (c map))
(defcfun (map-dremove "fa_map_dremove") map (a map-key) (b map))
(defcfun (map-add-entry "fa_map_add_entry") map (a pair) (b map))
(defcfun (map-set-entry "fa_map_set_entry") map (a pair) (b map))
(defcfun (map-remove-entry "fa_map_remove_entry") map (a pair) (b map))
(defcfun (map-get "fa_map_get") ptr (a map-key) (b map))
(defcfun (map-has-key "fa_map_has_key") :boolean (a map-key) (b map))
(defcfun (map-has-elem "fa_map_has_elem") :boolean (a ptr) (b map))
(defcfun (map-has-entry "fa_map_has_entry") :boolean (a pair) (b map))
(defcfun (map-is-submap-of "fa_map_is_submap_of") :boolean (a map) (b map))
(defcfun (map-is-proper-submap-of "fa_map_is_proper_submap_of") :boolean (a map) (b map))
(defcfun (map-sum "fa_map_sum") map (a map) (b map))
(defcfun (map-product "fa_map_product") map (a map) (b map))
(defcfun (map-difference "fa_map_difference") map (a map) (b map))
(defcfun (map-map "fa_map_map") map (a unary) (b ptr) (c map))
(defcfun (map-from-pair "fa_map_from_pair") map (a pair))
(defcfun (map-from-list "fa_map_from_list") map (a list))
(defcfun (map-to-list "fa_map_to_list") list (a map))

