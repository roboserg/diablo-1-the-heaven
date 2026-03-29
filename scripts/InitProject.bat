@if exist personal.props goto NEXT
@copy scripts\personal.props personal.props
:NEXT
@if exist theheaven.vcxproj.user goto END
@copy scripts\theheaven.vcxproj.user theheaven.vcxproj.user
:END
