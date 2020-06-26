MRuby::Gem::Specification.new('mruby-lunapurpura-clu') do |spec|
  spec.license = %q(MIT)
  spec.authors = [%q(Charlotte Koch),]
  spec.summary = %q(Bindings to Luna Purpura's CLU library)

  spec.cc.include_paths << "#{build.root}/src"
end
